#pragma once

#include "../handle.hpp"

namespace uvxx
{
	template <typename T>
	inline void handle::alloc()
	{
		m_uv = {(uv_handle_t*) std::malloc(sizeof(T)), uv_free};
	}
}
