#include "stream.hpp"

#include <uv.h>
#include "../yieldxx/task.hpp"
#include "private/run_callback.hpp"
#include "error.hpp"

namespace uvxx
{
	auto stream::is_readable() -> bool
	{
		return uv_is_readable(uv_stream());
	}

	auto stream::is_writable() -> bool
	{
		return uv_is_writable(uv_stream());
	}

	struct callbacks
	{
		std::function<void (size_t, uv_buf_t*)> alloc;
		std::function<void (ssize_t, const uv_buf_t*)> read;
	};

	inline void alloc_callback(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
	{
		auto cbs = (callbacks*) handle->data;
		cbs->alloc(suggested_size, buf);
	}

	inline void read_callback(uv_stream_t* handle, ssize_t nread, const uv_buf_t* buf)
	{
		auto cbs = (callbacks*) handle->data;
		cbs->read(nread, buf);
	}

	auto stream::read(xx::task& task) -> xx::generated<std::string>
	{
		return xx::generated<std::string>([&task, stream = uv_stream()](typename xx::generator<std::string>::yield&& yield)
		{
			// Note: coroutines have quite a small stack, therefore we need to allocate a respectively small buffer.
			char buffer[8 * 1024];
			struct
			{
				ssize_t nread;
				const uv_buf_t* buf;
			} res;

			auto cbs = callbacks{};
			stream->data = &cbs;

			cbs.alloc = [&](size_t, uv_buf_t* buf)
			{
				buf->base = buffer;
				buf->len = sizeof(buffer);
			};

			cbs.read = [&](ssize_t nread, const uv_buf_t* buf)
			{
				// From docs: nread might be 0, which does not indicate an error or EOF. This is equivalent to EAGAIN or EWOULDBLOCK under read(2).
				if (nread == 0)
					return;

				res.nread = nread;
				res.buf = buf;
				task.resume();
			};

			uv_read_start(stream, alloc_callback, read_callback);
			auto auto_read_stop = std::unique_ptr<uv_stream_t, decltype(uv_read_stop)*>{stream, &uv_read_stop};

			while (true) // until coroutine::stop gets thrown from yield() - when user destroys the generator.
			{
				task.yield();
				if (res.nread < 0)
				{
					if (res.nread == UV_EOF)
						break;
					else
						throw error{res.nread};
				}
				yield(std::string{res.buf->base, (size_t) res.nread});
			}
		});
	}

	struct async_write_data
	{
		uv_write_t uv;
		std::string data;
		uv_buf_t buf;
	};

	inline void delete_async_write_data(uv_write_t* req, int status)
	{
		std::unique_ptr<async_write_data>{(async_write_data*) req->data};
	}

	void stream::write(const std::string& data)
	{
		auto stream = uv_stream();

		auto wd = std::make_unique<async_write_data>();
		wd->data = data; // uv_write does not copy the data before returning, we need to preserve it until callback fires.
		wd->buf = uv_buf_init(&wd->data[0], wd->data.size());
		wd->uv.data = wd.get();

		uv_write(&wd->uv, stream, &wd->buf, 1, delete_async_write_data);

		wd.release();
	}
}
