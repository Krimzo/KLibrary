#include "examples.h"


int examples::video_reading_main( int argc, char** argv )
{
    std::string filepath = {};
    if ( argc == 2 )
    {
        filepath = argv[1];
    }
    else
    {
        kl::print<false>( "Video file path: " );
        std::getline( std::cin, filepath );
    }

    kl::VideoReader video_reader{ filepath };

    kl::console::clear();
    kl::print<false>( "Resize the console and press enter.." );
    std::cin.get();

    kl::Int2 console_size = kl::console::size() - kl::Int2( 0, 1 );
    kl::console::set_cursor_enabled( false );

    int frame_count = video_reader.frame_count();
    std::vector<std::string> ascii_frames;
    ascii_frames.reserve( frame_count );

    kl::console::clear();
    kl::Image video_frame;
    while ( video_reader.read_frame( video_frame ) )
    {
        ascii_frames.push_back( video_frame.as_ascii( console_size ) );
        kl::console::move_cursor( {} );
        kl::print<false>( "Processed: ", ascii_frames.size(), "/", frame_count );
    }

    kl::console::clear();
    float to_wait = 1.0f / video_reader.fps();
    while ( true )
    {
        for ( size_t i = 0; i < ascii_frames.size(); i++ )
        {
            auto start_time = kl::time::now();
            kl::console::dump( ascii_frames[i] );
            kl::console::set_title( kl::format( i + 1, "/", ascii_frames.size() ) );
            while ( kl::time::elapsed( start_time ) < to_wait );
        }
    }
    return 0;
}
