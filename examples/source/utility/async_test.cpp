#include "examples.h"


static std::vector<float> data( 100'000'000 );

static void clear_data()
{
    for ( auto& value : data )
        value = 0.0f;
}

template<typename F>
static float time_it( F const& func )
{
    auto start_time = kl::time::now();
    func();
    return kl::time::elapsed( start_time );
}

static float compute_function( size_t value )
{
    return sin( (float) value ) / sqrt( (float) value );
}

static void for_test()
{
    for ( size_t i = 0; i < data.size(); i++ )
        data[i] = compute_function( i );
}

static void async_test()
{
    kl::async_for<int>( 0, (int) data.size(), [&]( int i )
    {
        data[i] = compute_function( i );
    } );
}

int examples::async_test_main( int argc, char** argv )
{
    static size_t index = kl::random::gen_int( (int) data.size() );
    kl::print( "Random index: ", index, "\n" );

    clear_data();
    kl::print( "for time: ", time_it( for_test ) );
    kl::print( "for data[", index, "] = ", data[index], "\n" );

    clear_data();
    kl::print( "kl::async_for time: ", time_it( async_test ) );
    kl::print( "kl::async_for data[", index, "] = ", data[index], "\n" );

    return 0;
}
