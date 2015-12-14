#pragma once

#include <stdexcept>

namespace uvxx
{
	// TODO use uv_strerror and uv_err_name
	struct error: std::exception
	{
		error(ssize_t error) {}
	};
}
