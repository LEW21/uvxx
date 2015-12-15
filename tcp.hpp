#pragma once

#include "stream.hpp"

namespace uvxx
{
	struct tcp: public stream
	{
		auto uv_tcp() {return (uv_tcp_t*) uv_handle();}

		tcp() {}
		tcp(uv_loop_t* loop);

		auto accept() -> tcp;
		void bind(const std::string& ip, uint port, uint flags = 0);

		auto getsockname() -> std::string;
		auto getpeername() -> std::string;

		void nodelay(bool enable);
		void keepalive(bool enable, uint delay);
		void simultaneous_accepts(bool enable);

	private:
		// I don't think it's usable in the standard situations,
		// as std::function requires lambda copyability,
		// making it impossible to move a non-copyable object into
		// a spawn_tasked subtask.

		// However, it may get exposed in the future, if circumstances change.
		auto listen_and_accept(xx::task& task, int backlog) -> xx::generated<tcp>;
	};
}
