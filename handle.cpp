#include "handle.hpp"

#include <uv.h>
#include "../yieldxx/task.hpp"
#include "private/run_callback.hpp"

namespace uvxx
{
	void uv_free(uv_handle_t* handle)
	{
		uv_close(handle, (uv_close_cb) std::free);
	}

	void handle::close(xx::task& task)
	{
		auto handle = uv_handle();

		auto cb = std::function<void (uv_handle_t*)>
		{[&](uv_handle_t*){
			task.resume();
		}};
		handle->data = &cb;

		auto uv = m_uv.release();
		uv_close(uv, run_callback<uv_handle_t*>); task.yield();
		std::free(uv);
	}
}
