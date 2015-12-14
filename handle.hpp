#pragma once

#include <memory>
#include "error.hpp"
#include "uvfwd.hpp"

namespace xx
{
	struct task;
}

namespace uvxx
{
	void uv_free(uv_handle_t* handle);

	class handle
	{
		std::unique_ptr<uv_handle_t, decltype(uv_free)*> m_uv{nullptr, uv_free};

	protected:
		template <typename T>
		void alloc();

	public:
		auto uv_handle() {return m_uv.get();}

		void close(xx::task& task);
	};
}
