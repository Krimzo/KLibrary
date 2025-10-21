#include "examples.h"


static void draw_axis( kl::Image& frame, kl::RGB color = { 75, 75, 75 } )
{
    frame.draw_line( kl::Int2( 0, frame.height() / 2 ), { frame.width(), frame.height() / 2 }, color );
    frame.draw_line( kl::Int2( frame.width() / 2, 0 ), { frame.width() / 2, frame.height() }, color );
}

static void draw_data( kl::Image& frame, std::vector<kl::Int2> const& data, kl::RGB color = kl::colors::ORANGE )
{
    kl::Int2 half_size = frame.size() / 2;
    for ( auto& val : data )
    {
        kl::Int2 write_position = val * kl::Int2( 1, -1 ) + half_size;
        if ( frame.in_bounds( write_position ) )
            frame[write_position] = color;
    }
}

static void draw_line( kl::Image& frame, kl::Float2 equat, kl::RGB color = kl::colors::SKY )
{
    kl::Int2 half_size = frame.size() / 2;
    kl::Int2 pos1 = kl::Int2( -half_size.x, (int) ( -half_size.x * equat.x + equat.y ) ) * kl::Int2( 1, -1 ) + half_size;
    kl::Int2 pos2 = kl::Int2( half_size.x, (int) ( half_size.x * equat.x + equat.y ) ) * kl::Int2( 1, -1 ) + half_size;
    frame.draw_line( pos1, pos2, color );
}

static float calculate_offsets( std::vector<kl::Int2> const& data, kl::Float2 line_equat )
{
    float sum = 0.0f;
    for ( auto& val : data )
        sum += abs( val.y - ( val.x * line_equat.x + line_equat.y ) );
    return sum;
}

static void draw_offset( kl::Image& frame, std::vector<kl::Int2> const& data, kl::Float2 lineEquat, kl::RGB color = kl::colors::YELLOW )
{
    if ( !data.empty() )
    {
        static size_t data_index = 0;

        data_index = ( data_index + 1 ) % data.size();
        kl::Int2 half_size = frame.size() / 2;
        kl::Int2 pos1 = kl::Int2( data[data_index].x, data[data_index].y ) * kl::Int2( 1, -1 ) + half_size;
        kl::Int2 pos2 = kl::Int2( data[data_index].x, (int) ( data[data_index].x * lineEquat.x + lineEquat.y ) ) * kl::Int2( 1, -1 ) + half_size;
        frame.draw_line( pos1, pos2, color );
    }
}

static void calculate_improved_line( std::vector<kl::Int2> const& data, kl::Float2& line_equat )
{
    static kl::Float2 alter_x( 10.0f, 0.0f );
    static kl::Float2 alter_y( 0.0f, 10.0f );

    if ( static uint64_t last_data_size = 0; data.size() != last_data_size )
    {
        last_data_size = data.size();
        alter_x = { 10.0f, 0.0f };
        alter_y = { 0.0f, 10.0f };
        line_equat = { 1.0f, 0.0f };
    }

    float orig_sum = calculate_offsets( data, line_equat );

    if ( calculate_offsets( data, line_equat + alter_x ) < orig_sum )
    {
        line_equat += alter_x;
    }
    else if ( calculate_offsets( data, line_equat - alter_x ) < orig_sum )
    {
        line_equat -= alter_x;
    }
    else
    {
        alter_x *= 0.75f;
    }

    if ( calculate_offsets( data, line_equat + alter_y ) < orig_sum )
    {
        line_equat += alter_y;
    }
    else if ( calculate_offsets( data, line_equat - alter_y ) < orig_sum )
    {
        line_equat -= alter_y;
    }
    else
    {
        alter_y *= 0.75f;
    }
}

int examples::data_line_main( int argc, char** argv )
{
    kl::Window window{ "Data Line" };
    kl::Image frame{ window.size() };

    std::vector<kl::Int2> data;
    kl::Float2 line_equat( 1.0f, 0.0f );

    while ( window.process() )
    {
        frame.fill( kl::colors::GRAY );

        if ( window.keyboard.r.pressed() )
            data.clear();

        if ( window.mouse.left )
        {
            static kl::Int2 last_data;
            kl::Int2 new_data = window.mouse.position() * kl::Int2( 1, -1 );
            new_data -= frame.size() / kl::Int2( 2, -2 );
            if ( new_data != last_data )
            {
                data.push_back( new_data );
                last_data = new_data;
            }
        }

        draw_axis( frame );
        draw_data( frame, data );
        draw_line( frame, line_equat );
        draw_offset( frame, data, line_equat );

        calculate_improved_line( data, line_equat );

        window.draw_image( frame );
    }
    return 0;
}
