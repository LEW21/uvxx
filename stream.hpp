#pragma once

#include "handle.hpp"
#include "../yieldxx/generated.hpp"

namespace uvxx
{
	struct stream: public handle
	{
		auto uv_stream() {return (uv_stream_t*) uv_handle();}

		auto is_readable() -> bool;
		auto is_writable() -> bool;

		auto read(xx::task& task) -> xx::generated<std::string>;
		void write(const std::string& data);
	};
}
