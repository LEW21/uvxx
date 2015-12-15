#include "tcp.hpp"

#include <uv.h>
#include "private/alloc.hpp"

namespace uvxx
{
	tcp::tcp(uv_loop_t* loop)
	{
		alloc<uv_loop_t>();
		uv_tcp_init(loop, uv_tcp());
	}

	auto tcp::accept() -> tcp
	{
		auto client = tcp{uv_stream()->loop};
		uv_accept(uv_stream(), client.uv_stream());
		return client;
	}

	auto tcp::listen_and_accept(xx::task& task, int backlog) -> xx::generated<tcp>
	{
		auto stream = uv_stream();
		auto listener_func = listen(task, backlog).gen;

		return xx::generated<tcp>([stream, listener_func](xx::generator<tcp>::yield&& yield)
		{
			listener_func([&](client_connected connected){
				auto client = tcp{stream->loop};
				uv_accept(stream, client.uv_stream());
				yield(std::move(client));
			});
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
