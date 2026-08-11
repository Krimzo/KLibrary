#include "examples.h"


int examples::text_drawing_main( int argc, char** argv )
{
    kl::Window window{ "Text Drawing" };
    kl::GPU gpu{ window.ptr() };

    window.on_resize.emplace_back( [&]( kl::Int2 size )
        {
            gpu.resize_internal( size );
            gpu.set_viewport_size( size );
        } );
    window.set_dark_mode( true );
    window.maximize();

    kl::TextFormat format = gpu.create_text_format( L"roboto", DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, 25.0f );
    if ( !format )
        return 1;

    std::wstringstream message;
    for ( int i = 0; i < 20; i++ )
        message << i << "^2 = " << i * i << '\n';

    auto& text = gpu.text_batch.emplace_back();
    text.format = format;
    text.position = { -1.0f, 1.0f };
    text.rect_size = { 2.0f, 2.0f };
    text.hor_center = true;
    text.ver_center = true;
    text.data = message.str();

    while ( window.process() )
    {
        gpu.clear_internal( kl::colors::GRAY );
        gpu.draw_text_batch();
        gpu.swap_buffers( true );
    }
    return 0;
}
