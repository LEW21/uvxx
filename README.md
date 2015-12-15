# uvxx
[![Documentation Status](https://readthedocs.org/projects/uvxx/badge/?version=latest)](http://uvxx.readthedocs.org/en/latest/?badge=latest)

uvxx is a coroutine-based multi-platform C++ networking library that uses [yieldxx](http://github.com/LEW21/yieldxx) to provide a high-level synchronous API on top of the [libuv](http://github.com/libuv/libuv) event loop.

```c++
#include "yieldxx/task.hpp"
#include "uvxx/tcp.hpp"
#include <uv.h>

void handle_connection(uvxx::tcp& server)
{
	xx::spawn_task([&](xx::task&& task){
		auto client = server.accept();

		for (auto res : client.read(task))
			client.write(res); // echo!
	});
}

int main()
{
	auto loop = uv_default_loop();

	auto server = uvxx::tcp{loop};
	server.bind("::", 7000);

	xx::spawn_task([&](xx::task&& task){
		for (auto client_connected : server.listen(task, 128))
			handle_connection(server);
	});

	return uv_run(loop, UV_RUN_DEFAULT);
}
```

[Read the docs!](http://uvxx.readthedocs.org/en/latest/)

## Usage
```sh
git submodule add https://github.com/LEW21/yieldxx.git
git submodule add https://github.com/LEW21/uvxx.git
```

* Add yieldxx/coroutine_boost.cpp (with -lboost_context) or yieldxx/coroutine_thread.cpp (with -pthread / whatever is your std::thread using) to your build system.
* Add uvxx/{handle.cpp,stream.cpp,tcp.cpp} to your build system.
* Add -luv

```c++
#include "yieldxx/task.hpp"
#include "uvxx/tcp.hpp"
#include <uv.h>
```

## Requirements

* Fully C++14-compliant compiler. Tested on gcc 5.2 and clang 3.7.
* libuv
* yieldxx Boost backend: Boost.Context 1.58+

## License - GNU AGPLv3+

Note: License might get changed to MIT in the future. If you need it, create a issue on GitHub.

Copyright (C) 2015 Janusz Lewandowski

This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License along with this program. If not, see http://www.gnu.org/licenses/.
