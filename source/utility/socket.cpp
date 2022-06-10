#include <ws2tcpip.h>
#include "utility/socket.h"
#include "utility/console.h"

#pragma comment(lib,"Ws2_32.lib")


WSADATA kl::socket::m_WSAData = {};
bool kl::socket::m_WSAInit = false;
void kl::socket::initWSA() {
	if (!m_WSAInit) {
		if (WSAStartup(MAKEWORD(2, 2), &m_WSAData)) {
			kl::console::show();
			std::cout << "Could not startup WSA!";
			std::cin.get();
			exit(69);
		}
		m_WSAInit = true;
	}
}
void kl::socket::uninitWSA() {
	if (m_WSAInit) {
		WSACleanup();
		m_WSAInit = false;
	}
}

kl::socket::socket() {
	m_Socket = ::socket(AF_INET, SOCK_STREAM, NULL);
	if (m_Socket == INVALID_SOCKET) {
		kl::console::show();
		std::cout << "Could not create a socket!";
		std::cin.get();
		exit(69);
	}
	m_Address.sin_family = AF_INET;
	m_Address.sin_addr.s_addr = INADDR_ANY;
}
kl::socket::socket(const SOCKET& sock) : m_Socket(sock) {}
kl::socket::~socket() {
	close();
}

kl::socket::operator SOCKET() {
	return m_Socket;
}

void kl::socket::close() {
	closesocket(m_Socket);
}

void kl::socket::port(uint port) {
	m_Address.sin_port = htons(port);
}
void kl::socket::address(const std::string& addrs) {
	if (inet_pton(AF_INET, addrs.c_str(), &m_Address.sin_addr) != 1) {
		kl::console::show();
		std::cout << "Could not parse address \"" << addrs << "\"";
		std::cin.get();
		exit(69);
	}
}

uint kl::socket::port() const {
	return ntohs(m_Address.sin_port);
}
std::string kl::socket::address() const {
	char buffer[INET_ADDRSTRLEN] = {};
	inet_ntop(AF_INET, &m_Address.sin_addr, buffer, INET_ADDRSTRLEN);
	return buffer;
}

void kl::socket::bind() {
	if (::bind(m_Socket, (sockaddr*)&m_Address, sizeof(m_Address))) {
		kl::console::show();
		std::cout << "Could not bind socket!";
		std::cin.get();
		exit(69);
	}
}

void kl::socket::listen(int queueSize) {
	if (::listen(m_Socket, queueSize)) {
		kl::console::show();
		std::cout << "Could not listen on socket!";
		std::cin.get();
		exit(69);
	}
}

kl::socket kl::socket::accept() {
	int addrLen = sizeof(m_Address);
	SOCKET accepted = ::accept(m_Socket, (sockaddr*)&m_Address, &addrLen);
	if (accepted == INVALID_SOCKET) {
		kl::console::show();
		std::cout << "Bad socket accepted!";
		std::cin.get();
		exit(69);
	}
	return kl::socket(accepted);
}

void kl::socket::connect() {
	if (::connect(m_Socket, (sockaddr*)&m_Address, sizeof(m_Address))) {
		kl::console::show();
		std::cout << "Could not connect to socket!";
		std::cin.get();
		exit(69);
	}
}

int kl::socket::send(const void* data, uint byteSize) {
	return ::send(m_Socket, (const char*)data, byteSize, NULL);
}

int kl::socket::receive(void* buff, uint byteSize) {
	return recv(m_Socket, (char*)buff, byteSize, NULL);
}
