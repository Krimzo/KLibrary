#include "klibrary.h"


static int _socket_init = []
    {
        WSADATA wsa_data{};
        int result = ::WSAStartup( MAKEWORD( 2, 2 ), &wsa_data );
        kl::assert( result == 0, "Failed to initialize WSA" );
        return result;
    }( );

kl::Address::Address()
    : sockaddr_in()
{
    sin_family = AF_INET;
}

std::string kl::Address::address() const
{
    char buffer[INET_ADDRSTRLEN + 1] = {};
    return ::inet_ntop( AF_INET, &sin_addr, buffer, INET_ADDRSTRLEN );
}

int kl::Address::set_address( std::string_view const& address )
{
    return ::inet_pton( AF_INET, address.data(), &sin_addr );
}

int kl::Address::port() const
{
    return ( int ) ::ntohs( sin_port );
}

void kl::Address::set_port( int port )
{
    sin_port = ::htons( (u_short) port );
}

kl::TCPSocket::TCPSocket()
{
    open();
}

kl::TCPSocket::~TCPSocket() noexcept
{
    close();
}

kl::TCPSocket::operator bool() const
{
    return socket != INVALID_SOCKET;
}

void kl::TCPSocket::open()
{
    address.sin_addr.s_addr = INADDR_ANY;
    socket = ::socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
}

void kl::TCPSocket::close()
{
    ::closesocket( socket );
    socket = INVALID_SOCKET;
}

int kl::TCPSocket::server_bind()
{
    return ::bind( socket, (sockaddr*) &address, sizeof( Address ) );
}

int kl::TCPSocket::server_listen( int queue_size )
{
    return ::listen( socket, queue_size );
}

void kl::TCPSocket::server_accept( TCPSocket& out_client )
{
    int _ = sizeof( Address );
    out_client.socket = ::accept( socket, (sockaddr*) &address, &_ );
}

int kl::TCPSocket::client_connect()
{
    return ::connect( socket, (sockaddr*) &address, sizeof( Address ) );
}

int kl::TCPSocket::send( void const* data, int byte_size ) const
{
    return ::send( socket, (char const*) data, byte_size, NULL );
}

int kl::TCPSocket::receive( void* buff, int byte_size ) const
{
    return ::recv( socket, (char*) buff, byte_size, NULL );
}

uint64_t kl::TCPSocket::exhaust( std::vector<byte>& output, int buffer_size ) const
{
    uint64_t total_received = 0;
    std::vector<byte> receiver_buffer( buffer_size );
    for ( int received; ( received = receive( receiver_buffer.data(), buffer_size ) ) > 0;)
    {
        output.insert( output.end(), receiver_buffer.begin(), receiver_buffer.begin() + received );
        total_received += received;
    }
    return total_received;
}

kl::UDPSocket::UDPSocket()
{
    open();
}

kl::UDPSocket::~UDPSocket()
{
    close();
}

kl::UDPSocket::operator bool() const
{
    return socket != INVALID_SOCKET;
}

void kl::UDPSocket::open()
{
    address.sin_addr.s_addr = INADDR_ANY;
    socket = ::socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
}

void kl::UDPSocket::close()
{
    ::closesocket( socket );
    socket = INVALID_SOCKET;
}

int kl::UDPSocket::server_bind()
{
    return ::bind( socket, (sockaddr*) &address, sizeof( Address ) );
}

int kl::UDPSocket::send( void const* data, int byte_size, Address const& address ) const
{
    return ::sendto( socket, (char const*) data, byte_size, NULL, (sockaddr const*) &address, sizeof( Address ) );
}

int kl::UDPSocket::receive( void* buff, int byte_size, Address& address ) const
{
    int address_length = sizeof( Address );
    return ::recvfrom( socket, (char*) buff, byte_size, NULL, (sockaddr*) &address, &address_length );
}
