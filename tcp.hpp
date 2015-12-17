#pragma once

#include "stream.hpp"

namespace uvxx
{
	struct tcp: public stream
	{
		auto uv_tcp() {return (uv_tcp_t*) uv_handle();}

		tcp() {}
		tcp(uv_loop_t* loop);

		void bind(const std::string& ip, uint port, uint flags = 0);
		auto listen(xx::task& task, int backlog) -> xx::generated<tcp>;

		auto getsockname() -> std::string;
		auto getpeername() -> std::string;

		void nodelay(bool enable);
		void keepalive(bool enable, uint delay);
		void simultaneous_accepts(bool enable);
	};
}
