#include <ws2tcpip.h>
#include "utility/socket.h"
#include "utility/console.h"

#pragma comment(lib,"Ws2_32.lib")


static bool WSAInited = false;
const kl::String kl::Socket::Self = "127.0.0.1";

void kl::Socket::Init() {
	if (!WSAInited) {
		WSADATA WSAData = {};
		Assert(WSAStartup(MAKEWORD(2, 2), &WSAData), "Failed to initialize WSA");
		WSAInited = true;
	}
}

void kl::Socket::Uninit() {
	if (WSAInited) {
		WSACleanup();
		WSAInited = false;
	}
}

kl::Socket::Socket() {
	m_Socket = ::socket(AF_INET, SOCK_STREAM, NULL);
	Assert(m_Socket == INVALID_SOCKET, "Failed to create socket");
	m_Address.sin_family = AF_INET;
	m_Address.sin_addr.s_addr = INADDR_ANY;
}

kl::Socket::Socket(const String& address, uint port) : kl::Socket() {
	setAddress(address);
	setPort(port);
}

kl::Socket::Socket(uint64 socket) : m_Socket(socket) {}

kl::Socket::~Socket() {
	close();
}

kl::Socket::operator kl::uint64() {
	return m_Socket;
}

void kl::Socket::close() {
	closesocket(m_Socket);
}

kl::String kl::Socket::getAddress() const {
	char buffer[INET_ADDRSTRLEN] = {};
	inet_ntop(AF_INET, &m_Address.sin_addr, buffer, INET_ADDRSTRLEN);
	return buffer;
}

void kl::Socket::setAddress(const String& addrs) {
	Assert(inet_pton(AF_INET, addrs.c_str(), &m_Address.sin_addr) != 1, "Could not convert address");
}

kl::uint kl::Socket::getPort() const {
	return ntohs(m_Address.sin_port);
}

void kl::Socket::setPort(uint port) {
	m_Address.sin_port = htons(port);
}

void kl::Socket::listen(uint queueSize) {
	Assert(::bind(m_Socket, (sockaddr*) &m_Address, sizeof(m_Address)), "Could not bind socket");
	Assert(::listen(m_Socket, queueSize), "Could not listen on socket");
}

kl::Socket kl::Socket::accept() {
	int addrLen = sizeof(m_Address);
	uint64 accepted = ::accept(m_Socket, (sockaddr*) &m_Address, &addrLen);
	Assert(accepted == INVALID_SOCKET, "Could not accept socket");
	return accepted;
}

void kl::Socket::connect() {
	Assert(::connect(m_Socket, (sockaddr*) &m_Address, sizeof(m_Address)), "Could not connect to socket");
}

int kl::Socket::send(const void* data, uint byteSize) const {
	return ::send(m_Socket, (const char*) data, byteSize, NULL);
}

int kl::Socket::receive(void* buff, uint byteSize) const {
	return recv(m_Socket, (char*) buff, byteSize, NULL);
}
