#include "../../yieldxx/task.hpp"
#include "../tcp.hpp"
#include <uv.h>

void handle_connection(uvxx::tcp&& client)
{
	xx::spawn_task([client = std::move(client)](xx::task&& task) mutable {
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
		for (auto& client : server.listen(task, 128))
			handle_connection(std::move(client));
	});

	return uv_run(loop, UV_RUN_DEFAULT);
}
