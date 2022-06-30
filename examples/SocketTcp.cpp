#include "KrimzLib.h"


void Server() {
	kl::socket server(kl::socket::self, 1709);

	server.listen(1);

	kl::socket client = server.accept();
	client.send<kl::float3>({ 1.0f, 2.0f, 3.0f });
}

void Client() {
	kl::socket client(kl::socket::self, 1709);

	client.connect();

	kl::float3 res;
	client.receive(res);
	kl::print("Result: ", res);
}

int main() {
	kl::socket::init();

	std::thread(Server).detach();
	kl::time::wait(0.25);
	std::thread(Client).detach();


	std::cin.get();
}
