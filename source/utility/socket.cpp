#include <ws2tcpip.h>
#include "utility/socket.h"
#include "utility/console.h"

#pragma comment(lib,"Ws2_32.lib")


static bool WSAInited = false;
const std::string kl::socket::self = "127.0.0.1";

void kl::socket::init() {
	if (!WSAInited) {
		WSADATA WSAData = {};
		kl::console::error(WSAStartup(MAKEWORD(2, 2), &WSAData), "Failed to initialize WSA");
		WSAInited = true;
	}
}
void kl::socket::uninit() {
	if (WSAInited) {
		WSACleanup();
		WSAInited = false;
	}
}

kl::socket::socket() {
	m_Socket = ::socket(AF_INET, SOCK_STREAM, NULL);
	kl::console::error(m_Socket == INVALID_SOCKET, "Failed to create socket");
	m_Address.sin_family = AF_INET;
	m_Address.sin_addr.s_addr = INADDR_ANY;
}
kl::socket::socket(const std::string& address, uint port) : kl::socket() {
	this->address(address);
	this->port(port);
}
kl::socket::socket(uint64 socket) : m_Socket(socket) {}
kl::socket::~socket() {
	close();
}

kl::socket::operator uint64() {
	return m_Socket;
}

void kl::socket::close() {
	closesocket(m_Socket);
}

std::string kl::socket::address() const {
	char buffer[INET_ADDRSTRLEN] = {};
	inet_ntop(AF_INET, &m_Address.sin_addr, buffer, INET_ADDRSTRLEN);
	return buffer;
}

void kl::socket::address(const std::string& addrs) {
	kl::console::error(inet_pton(AF_INET, addrs.c_str(), &m_Address.sin_addr) != 1, "Could not convert address");
}

uint kl::socket::port() const {
	return ntohs(m_Address.sin_port);
}

void kl::socket::port(uint port) {
	m_Address.sin_port = htons(port);
}

void kl::socket::listen(uint queueSize) {
	kl::console::error(::bind(m_Socket, (sockaddr*) &m_Address, sizeof(m_Address)), "Could not bind socket");
	kl::console::error(::listen(m_Socket, queueSize), "Could not listen on socket");
}

kl::socket kl::socket::accept() {
	int addrLen = sizeof(m_Address);
	uint64 accepted = ::accept(m_Socket, (sockaddr*) &m_Address, &addrLen);
	kl::console::error(accepted == INVALID_SOCKET, "Could not accept socket");
	return accepted;
}

void kl::socket::connect() {
	kl::console::error(::connect(m_Socket, (sockaddr*) &m_Address, sizeof(m_Address)), "Could not connect to socket");
}

int kl::socket::send(const void* data, uint byteSize) const {
	return ::send(m_Socket, (const char*) data, byteSize, NULL);
}

int kl::socket::receive(void* buff, uint byteSize) const {
	return recv(m_Socket, (char*) buff, byteSize, NULL);
}
