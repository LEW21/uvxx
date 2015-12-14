#pragma once

namespace uvxx
{
	template <typename O, typename... Arg>
	inline void run_callback(O o, Arg... arg)
	{
		if (!o->data)
			return;

		auto& cb = *((std::function<void (O, Arg...)>*) o->data);
		cb(std::forward<O>(o), std::forward<Arg>(arg)...);
	}
}
