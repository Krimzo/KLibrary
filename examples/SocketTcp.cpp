#include "KrimzLib.h"


void Server() {
	kl::socket server;

	server.setPort(1709);

	server.bind();
	server.listen(2);

	printf("Server: Waiting for clients..\n");
	kl::socket client = server.accept();
	printf("Server: Client connected!\n");

	const char mess[10] = "It works!";
	client.send(mess, sizeof(mess));
}

void Client() {
	kl::socket client;

	client.setAddr("127.0.0.1");
	client.setPort(1709);

	client.connect();

	char buff[10] = {};
	while (client.recieve(buff, sizeof(buff))) {
		printf("Client: Server sent: %s\n", buff);
	}
}

int main() {
	kl::socket::initWSA();

	std::thread(Server).detach();
	std::thread(Client).detach();


	std::cin.get();
	kl::socket::uninitWSA();
}
