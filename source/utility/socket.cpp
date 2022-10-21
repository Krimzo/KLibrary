#include "utility/socket.h"

#include "utility/console.h"
#include <atomic>


static std::atomic wsa_initialised = false;

void kl::socket::init_utility()
{
    if (!wsa_initialised) {
        WSADATA wsa_data = {};
        assert(WSAStartup(MAKEWORD(2, 2), &wsa_data), "Failed to initialize WSA");
        wsa_initialised = true;
    }
}

void kl::socket::free_utility()
{
    if (wsa_initialised) {
        WSACleanup();
        wsa_initialised = false;
    }
}

kl::socket::socket()
{
    socket_ = ::socket(AF_INET, SOCK_STREAM, NULL);
    assert(socket_ == INVALID_SOCKET, "Failed to create socket");
    address_.sin_family = AF_INET;
    address_.sin_addr.s_addr = INADDR_ANY;
}

kl::socket::socket(const std::string& address, const uint32_t port)
    : socket()
{
    set_address(address);
    set_port(port);
}

kl::socket::socket(const uint64_t socket)
    : socket_(socket)
{}

kl::socket::~socket()
{
    close();
}

kl::socket::operator uint64_t() const
{
    return socket_;
}

void kl::socket::close() const
{
    closesocket(socket_);
}

std::string kl::socket::get_address() const
{
    char buffer[INET_ADDRSTRLEN] = {};
    inet_ntop(AF_INET, &address_.sin_addr, buffer, INET_ADDRSTRLEN);
    return buffer;
}

void kl::socket::set_address(const std::string& address)
{
    assert(inet_pton(AF_INET, address.c_str(), &address_.sin_addr) != 1, "Could not convert address");
}

uint32_t kl::socket::get_port() const
{
    return ntohs(address_.sin_port);
}

void kl::socket::set_port(const uint32_t port)
{
    address_.sin_port = htons(static_cast<u_short>(port));
}

void kl::socket::listen(const uint32_t queue_size)
{
    assert(bind(socket_, reinterpret_cast<sockaddr*>(&address_), sizeof(address_)), "Could not bind socket");
    assert(::listen(socket_, static_cast<int>(queue_size)), "Could not listen on socket");
}

kl::socket kl::socket::accept()
{
    int address_length = sizeof(address_);
    const uint64_t accepted = ::accept(socket_, reinterpret_cast<sockaddr*>(&address_), &address_length);
    assert(accepted == INVALID_SOCKET, "Could not accept socket");
    return socket(accepted);
}

void kl::socket::connect()
{
    assert(::connect(socket_, reinterpret_cast<sockaddr*>(&address_), sizeof(address_)), "Could not connect to socket");
}

int kl::socket::send(const void* data, const uint32_t byte_size) const
{
    return ::send(socket_, static_cast<const char*>(data), static_cast<int>(byte_size), NULL);
}

int kl::socket::receive(void* buff, const uint32_t byte_size) const
{
    return recv(socket_, static_cast<char*>(buff), static_cast<int>(byte_size), NULL);
}
