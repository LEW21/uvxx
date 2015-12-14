#include "../../yieldxx/task.hpp"
#include "../tcp.hpp"
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
