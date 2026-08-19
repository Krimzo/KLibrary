#include "examples.h"


int examples::raster_example_main( int argc, char** argv )
{
    kl::Window window{ "Raster Example" };
    kl::GPU gpu{ window.ptr() };

    window.on_resize.emplace_back( [&]( int2 size )
        {
            gpu.resize_internal( size );
            gpu.set_viewport_size( size );

            gpu.raster_batch.clear();
            gpu.raster_batch.emplace_back( new kl::RasterLine{ float2{ 0.0f, 0.0f }, float2{ -0.5f, 0.5f }, kl::colors::RED } );
            gpu.raster_batch.emplace_back( new kl::RasterLine{ float2{ -0.5f, 0.5f }, float2{ 0.0f, 0.5f }, kl::colors::GREEN } );
            gpu.raster_batch.emplace_back( new kl::RasterLine{ float2{ 0.0f, 0.5f }, float2{ 0.0f, 0.0f }, kl::colors::BLUE } );
            gpu.raster_batch.emplace_back( new kl::RasterRectangle{ float2{ 0.0f, 0.75f }, float2{ 0.5f, 0.5f }, kl::colors::CRIMSON, false } );
            gpu.raster_batch.emplace_back( new kl::RasterRectangle{ float2{ 0.5f, 0.5f }, float2{ 0.75f, 0.25f }, kl::colors::SKY, true, { 25.0f, 25.0f } } );
            gpu.raster_batch.emplace_back( new kl::RasterEllipse{ float2{ 0.0f, 0.0f }, float2{ 0.5f * size.y / size.x, 0.5f }, kl::colors::WHEAT, false, 2.0f } );
            gpu.raster_batch.emplace_back( new kl::RasterEllipse{ float2{ -0.5f, -0.5f }, float2{ 0.5f * size.y / size.x, 0.5f }, kl::colors::WHITE, true } );
        } );
    window.resize( kl::SCREEN_SIZE / 2 );

    while ( window.process() )
    {
        gpu.clear_internal( kl::RGB{ 30, 30, 30 } );
        gpu.draw_raster_batch();
        gpu.swap_buffers( true );
    }
    return 0;
}
