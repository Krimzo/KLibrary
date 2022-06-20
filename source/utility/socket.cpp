#include <ws2tcpip.h>
#include "utility/socket.h"
#include "utility/console.h"

#pragma comment(lib,"Ws2_32.lib")


WSADATA kl::socket::m_WSAData = {};
bool kl::socket::m_WSAInit = false;
void kl::socket::initWSA() {
	if (!m_WSAInit) {
		kl::console::error(WSAStartup(MAKEWORD(2, 2), &m_WSAData), "Failed to initialize WSA");
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
	kl::console::error(m_Socket == INVALID_SOCKET, "Failed to create socket");
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
	kl::console::error(inet_pton(AF_INET, addrs.c_str(), &m_Address.sin_addr) != 1, "Could not convert address");
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
	kl::console::error(::bind(m_Socket, (sockaddr*)&m_Address, sizeof(m_Address)), "Could not bind socket");
}

void kl::socket::listen(int queueSize) {
	kl::console::error(::listen(m_Socket, queueSize), "Could not listen on socket");
}

kl::socket kl::socket::accept() {
	int addrLen = sizeof(m_Address);
	SOCKET accepted = ::accept(m_Socket, (sockaddr*)&m_Address, &addrLen);
	kl::console::error(accepted == INVALID_SOCKET, "Could not accept socket");
	return kl::socket(accepted);
}

void kl::socket::connect() {
	kl::console::error(::connect(m_Socket, (sockaddr*)&m_Address, sizeof(m_Address)), "Could not connect to socket");
}

int kl::socket::send(const void* data, uint byteSize) {
	return ::send(m_Socket, (const char*)data, byteSize, NULL);
}

int kl::socket::receive(void* buff, uint byteSize) {
	return recv(m_Socket, (char*)buff, byteSize, NULL);
}
