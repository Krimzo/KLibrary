#include "examples.h"


struct Stick
{
    int value = 0;
    kl::RGB color;
};

static std::vector<Stick> generate_sticks( int count, int min_value_incl, int max_value_excl )
{
    int stored_min_value = max_value_excl;
    int stored_max_value = min_value_incl;

    std::vector<Stick> sticks( count );
    for ( auto& [value, color] : sticks )
    {
        value = kl::random::gen_int( min_value_incl, max_value_excl );
        stored_min_value = std::min( stored_min_value, value );
        stored_max_value = std::max( stored_max_value, value );
    }

    for ( auto& [value, color] : sticks )
    {
        byte gray_value = (byte) ( kl::unlerp( (float) value, (float) stored_min_value, (float) stored_max_value ) * 255.0f );
        color = { gray_value, gray_value, gray_value };
    }
    return sticks;
}

static void draw_sticks( kl::Image& frame, std::vector<Stick> const& sticks )
{
    for ( int i = 0; i < (int) sticks.size(); i++ )
        frame.draw_line( { i, frame.height() - 1 }, { i, frame.height() - 1 - sticks[i].value }, sticks[i].color );
}

static std::string OUTPUT_VIDEO_PATH = "./media/generated_video.mp4";

int examples::video_writing_main( int argc, char** argv )
{
    kl::VideoWriter video_writer{ OUTPUT_VIDEO_PATH, kl::VideoType::h264(), { 1920, 1080 }, 60, 5.0f, 0 };

    kl::Image frame = { video_writer.frame_size() };
    std::vector<Stick> sticks = generate_sticks( frame.width(), 1, frame.height() );

    for ( size_t i = 0; i < sticks.size() - 1; i++ )
    {
        for ( size_t j = i + 1; j < sticks.size(); j++ )
        {
            if ( sticks[j].value < sticks[i].value )
                std::swap( sticks[i], sticks[j] );
        }
        frame.fill( kl::colors::GRAY );
        draw_sticks( frame, sticks );
        video_writer.add_frame( frame );
        kl::print( "[", ( i * 100.0f ) / ( sticks.size() - 2.0f ), "%]" );
    }

    video_writer.finalize();
    return 0;
}
