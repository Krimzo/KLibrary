#include "examples.h"


int examples::hashing_main( int argc, char** argv )
{
    std::string some_text = "This is a test!";
    kl::Float3 some_data = { 1.0f, 2.0f, 3.0f };

    kl::print( kl::hash_str( some_text ) );
    kl::print( kl::hash_obj( some_data ) );

    return 0;
}
