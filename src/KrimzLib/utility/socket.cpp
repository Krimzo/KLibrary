#include <ws2tcpip.h>
#include "KrimzLib/utility/socket.h"

#pragma comment(lib,"Ws2_32.lib")


WSADATA kl::socket::wsaData = {};
bool kl::socket::wsaInited = false;

// WSA setup
void kl::socket::initWSA() {
	if (!wsaInited) {
		if (WSAStartup(MAKEWORD(2, 2), &wsaData)) {
			std::cout << "Could not startup WSA!";
			std::cin.get();
			exit(69);
		}
		wsaInited = true;
	}
}
void kl::socket::uninitWSA() {
	if (wsaInited) {
		WSACleanup();
		wsaInited = false;
	}
}

kl::socket::socket() {
	sock = ::socket(AF_INET, SOCK_STREAM, NULL);
	if (sock == INVALID_SOCKET) {
		std::cout << "Could not create a socket!";
		std::cin.get();
		exit(69);
	}
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
}
kl::socket::socket(SOCKET sock) {
	this->sock = sock;
}
kl::socket::~socket() {
	this->close();
}

kl::socket::operator SOCKET() {
	return sock;
}

// Closes the socket
void kl::socket::close() {
	closesocket(sock);
}

// Info setters
void kl::socket::setPort(uint16_t port) {
	addr.sin_port = htons(port);
}
void kl::socket::setAddr(const std::string& addrs) {
	if (inet_pton(AF_INET, addrs.c_str(), &addr.sin_addr) != 1) {
		std::cout << "Could not parse address \"" << addrs << "\"";
		std::cin.get();
		exit(69);
	}
}

// Binds the socket
void kl::socket::bind() {
	if (::bind(sock, (sockaddr*)&addr, sizeof(addr))) {
		std::cout << "Could not bind socket!";
		std::cin.get();
		exit(69);
	}
}

// Sets socket mode to listen
void kl::socket::listen(int queueSize) {
	if (::listen(sock, queueSize)) {
		std::cout << "Could not listen on socket!";
		std::cin.get();
		exit(69);
	}
}

// Accepts new connections
kl::socket kl::socket::accept() {
	int addrLen = sizeof(addr);
	SOCKET accepted = ::accept(sock, (sockaddr*)&addr, &addrLen);
	if (accepted == INVALID_SOCKET) {
		std::cout << "Bad socket accepted!";
		std::cin.get();
		exit(69);
	}
	return kl::socket(accepted);
}

// Connects to a socket
void kl::socket::connect() {
	if (::connect(sock, (sockaddr*)&addr, sizeof(addr))) {
		std::cout << "Could not connect to socket!";
		std::cin.get();
		exit(69);
	}
}

// Sends data to socket
int kl::socket::send(const void* data, int byteSize) {
	return ::send(sock, (const char*)data, byteSize, NULL);
}

// Receives data from socket
int kl::socket::recieve(void* buff, int byteSize) {
	return recv(sock, (char*)buff, byteSize, NULL);
}
