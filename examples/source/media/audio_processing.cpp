#include "examples.h"


int examples::audio_processing_main( int argc, char** argv )
{
    kl::Audio chicken_wav;
    chicken_wav.load_from_file( "./media/chicken.wav" );

    kl::Audio chicken_mp3;
    chicken_mp3.sample_rate = 150'000;
    std::string mp3_data = kl::read_file_string( "./media/chicken.mp3" );
    chicken_mp3.load_from_memory( mp3_data.data(), mp3_data.size() );

    kl::print( chicken_wav.size(), " samples, ", chicken_wav.duration_seconds(), " seconds" );
    kl::print( chicken_mp3.size(), " samples, ", chicken_mp3.duration_seconds(), " seconds" );

    chicken_wav.save_to_file( "./media/chicken_1.mp3", kl::AudioType::MP3 );
    chicken_mp3.save_to_file( "./media/chicken_2.mp3", kl::AudioType::MP3 );

    return 0;
}
