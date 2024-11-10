#include "examples.h"


int examples::encryption_main( int argc, char** argv )
{
    kl::Encryptor encryptor;
    kl::print( encryptor );

    std::string data_0 = "Hello, World!";
    kl::Float4x4 data_1 = kl::Float4x4::scaling( { 1.0f, 2.0f, 3.0f } );
    kl::print<false>( kl::colors::SKY );
    kl::print( data_0 );
    kl::print( data_1 );

    encryptor.run_pass( data_0.data(), data_0.size() );
    encryptor.run_pass( data_1 );
    kl::print<false>( kl::colors::ORANGE );
    kl::print( data_0 );
    kl::print( data_1 );

    encryptor.run_pass( data_0.data(), data_0.size() );
    encryptor.run_pass( data_1 );
    kl::print<false>( kl::colors::CYAN );
    kl::print( data_0 );
    kl::print( data_1 );

    kl::print<false>( kl::colors::CONSOLE );
    return 0;
}
