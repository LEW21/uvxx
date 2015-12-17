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

	void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
		buf->base = new char[suggested_size];
		buf->len = suggested_size;
	}

	auto stream::read(xx::task& task) -> xx::generated<std::string>
	{
		return xx::generated<std::string>([&, stream = uv_stream()](typename xx::generator<std::string>::yield&& yield)
		{
			struct
			{
				ssize_t nread;
				const uv_buf_t* buf;
			} res;

			auto cb = std::function<void (uv_stream_t*, ssize_t, const uv_buf_t*)>
			{[&](uv_stream_t*, ssize_t nread, const uv_buf_t* buf){
				// From docs: nread might be 0, which does not indicate an error or EOF. This is equivalent to EAGAIN or EWOULDBLOCK under read(2).
				if (nread == 0)
					return;

				res.nread = nread;
				res.buf = buf;
				task.resume();
			}};
			stream->data = &cb;

			uv_read_start((uv_stream_t*) stream, alloc_buffer, run_callback<uv_stream_t*, ssize_t, const uv_buf_t*>);

			// Stop might get thrown from yield() when user destroys the generator.
			try
			{
				while (true)
				{
					task.yield();
					if (res.nread < 0)
						break;
					yield(std::string{res.buf->base, (size_t) res.nread});

					if (res.buf->base)
						delete[] res.buf->base;
				}
			}
			catch (xx::coroutine::stop&) {}

			if (res.buf->base)
				delete[] res.buf->base;

			uv_read_stop((uv_stream_t*) stream);

			stream->data = 0;

			if (res.nread < 0 && res.nread != UV_EOF)
				throw error{res.nread};
		});
	}

	struct async_write_data
	{
		uv_write_t uv;
		std::string data;
		uv_buf_t buf;
	};

	void delete_async_write_data(uv_write_t* req, int status)
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
