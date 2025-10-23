#include "examples.h"


void server_tcp()
{
    kl::TCPSocket server;
    server.address.set_port( 1709 );
    server.server_bind();
    server.server_listen( 1 );

    kl::TCPSocket client;
    server.server_accept( client );

    client.send<kl::Float3>( { 1.1f, 2.2f, 3.3f } );
    kl::print( "TCP data sent!" );
}

void client_tcp()
{
    kl::time::wait( 0.25f );

    kl::TCPSocket client;
    client.address.set_port( 1709 );
    client.address.set_address( "127.0.0.1" );
    client.client_connect();

    if ( auto opt_result = client.receive<kl::Float3>() )
        kl::print( "TCP data received: ", *opt_result );
}

void server_udp()
{
    kl::UDPSocket server;
    server.address.set_port( 1709 );
    server.server_bind();

    int recieve_data{};
    kl::Address address{};
    server.receive( recieve_data, address );

    kl::Float3 send_data{ 5.0f, 1.0f, 2.0f };
    server.send( send_data, address );
    kl::print( "UDP data sent!" );
}

void client_udp()
{
    kl::time::wait( 0.25f );
    kl::UDPSocket client;

    kl::Address host_address{};
    host_address.set_port( 1709 );
    host_address.set_address( "127.0.0.1" );

    int send_data = 1;
    client.send( send_data, host_address );

    kl::Address rec_address;
    if ( auto opt_result = client.receive<kl::Float3>( rec_address ) )
        kl::print( "UDP data received: ", *opt_result, " from ", rec_address.address(), ":", rec_address.port() );
}

int examples::sockets_main( int argc, char** argv )
{
    {
        kl::print( "Testing TCP" );
        std::vector<std::jthread> threads{};
        threads.emplace_back( server_tcp );
        threads.emplace_back( client_tcp );
    }
    {
        kl::print( "Testing UDP" );
        std::vector<std::jthread> threads{};
        threads.emplace_back( server_udp );
        threads.emplace_back( client_udp );
    }
    return 0;
}
