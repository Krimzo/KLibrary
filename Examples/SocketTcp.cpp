#include "KrimzLib.h"


static void Server() {
	kl::Socket server(kl::Socket::Self, 1709);

	server.listen(1);

	kl::Socket client = server.accept();
	client.send<kl::Float3>({ 1.0f, 2.0f, 3.0f });
}

static void Client() {
	kl::Socket client(kl::Socket::Self, 1709);

	client.connect();

	kl::Float3 res;
	client.receive(res);
	kl::Print("Result: ", res);
}

int main() {
	kl::Socket::Init();

	kl::Thread(Server).detach();
	kl::Time::Wait(0.25f);

	kl::Thread(Client).detach();

	kl::Get();

	kl::Socket::Uninit();
}
