#include "tcp.hpp"

#include <uv.h>
#include "../yieldxx/task.hpp"
#include "private/run_callback.hpp"
#include "private/alloc.hpp"

namespace uvxx
{
	tcp::tcp(uv_loop_t* loop)
	{
		alloc<uv_tcp_t>();
		uv_tcp_init(loop, uv_tcp());
	}

	auto tcp::listen(xx::task& task, int backlog) -> xx::generated<tcp>
	{
		return xx::generated<tcp>([this, &task, backlog, stream = uv_stream()](xx::generator<tcp>::yield&& yield)
		{
			int status;

			auto cb = std::function<void (uv_stream_t*, int)>
			{[&](uv_stream_t*, int s){
				status = s;
				task.resume();
			}};
			stream->data = &cb;

			uv_listen((uv_stream_t*) stream, backlog, run_callback<uv_stream_t*, int>);

			// Stop with coroutine::stop exception.
			try
			{
				while (true)
				{
					task.yield();
					if (status < 0)
						break;
					auto client = tcp{stream->loop};
					uv_accept(stream, client.uv_stream());
					yield(std::move(client));
				}
			}
			catch (xx::coroutine::stop&) {}

			// Unfortunately BSD sockets (and therefore libuv) don't support unlistening.
			// However, we won't crash, run_callback will simply do nothing.

			stream->data = 0;

			if (status < 0)
				throw error{status};
		});
	}

	void tcp::bind(const std::string& ip, uint port, uint flags)
	{
		if (ip.find_first_of(":") != std::string::npos)
		{
			struct sockaddr_in6 addr;
			uv_ip6_addr(ip.c_str(), port, &addr);
			uv_tcp_bind(uv_tcp(), (const struct sockaddr*)&addr, flags);
		}
		else
		{
			struct sockaddr_in addr;
			uv_ip4_addr(ip.c_str(), port, &addr);
			uv_tcp_bind(uv_tcp(), (const struct sockaddr*)&addr, flags);
		}
	}

	inline auto sockaddr_to_string(sockaddr_storage& addr) -> std::string
	{
		char name[512] = "";

		if (addr.ss_family == AF_INET)
			uv_ip4_name((sockaddr_in*) &addr, name, sizeof(name));
		else if (addr.ss_family == AF_INET6)
			uv_ip6_name((sockaddr_in6*) &addr, name, sizeof(name));

		return name;
	}

	auto tcp::getpeername() -> std::string
	{
		sockaddr_storage a;
		int len = sizeof(a);
		uv_tcp_getpeername(uv_tcp(), (sockaddr*) &a, &len);
		return sockaddr_to_string(a);
	}

	auto tcp::getsockname() -> std::string
	{
		sockaddr_storage a;
		int len = sizeof(a);
		uv_tcp_getsockname(uv_tcp(), (sockaddr*) &a, &len);
		return sockaddr_to_string(a);
	}

	void tcp::nodelay(bool enable)
	{
		uv_tcp_nodelay(uv_tcp(), enable);
	}

	void tcp::keepalive(bool enable, uint delay)
	{
		uv_tcp_keepalive(uv_tcp(), enable, delay);
	}

	void tcp::simultaneous_accepts(bool enable)
	{
		uv_tcp_simultaneous_accepts(uv_tcp(), enable);
	}
}
