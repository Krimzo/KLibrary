#include "examples.h"


int examples::compute_shaders2_main( int argc, char** argv )
{
    kl::Window window{ "Compute shaders 2" };
    kl::GPU gpu{ window.ptr() };
    kl::Timer timer;

    window.set_resizeable( false );
    gpu.resize_to_window( window.ptr() );

    kl::dx::DepthState depth_state = gpu.create_depth_state( false, false, false );
    gpu.bind_depth_state( depth_state );

    kl::dx::Texture render_texture = gpu.create_texture( kl::Image{ int2{ 1600, 900 } }, true, true );
    kl::dx::TargetView target_view = gpu.create_target_view( render_texture, nullptr );
    kl::dx::AccessView access_view = gpu.create_access_view( render_texture, nullptr );
    kl::dx::ShaderView shader_view = gpu.create_shader_view( render_texture, nullptr );

    std::string shader_sources = kl::read_file_string( "shaders/compute_test2.hlsl" );
    kl::Shaders shaders = gpu.create_shaders( shader_sources );
    kl::ComputeShader compute_shader = gpu.create_compute_shader( shader_sources );
    gpu.bind_shaders( shaders );
    gpu.bind_compute_shader( compute_shader.shader );

    kl::dx::Buffer screen_mesh = gpu.create_screen_mesh();

    while ( window.process() )
    {
        timer.update();

        struct alignas( 16 ) CB
        {
            float4 MISC_DATA;
        } cb = {};

        cb.MISC_DATA.xy() = window.mouse.position();
        compute_shader.upload( cb );

        gpu.clear_target_view( target_view, kl::colors::GRAY );

        gpu.bind_access_view_for_compute_shader( access_view, 0 );
        gpu.dispatch_compute_shader( kl::ceildiv<32>( window.width() ), kl::ceildiv<32>( window.height() ) );
        gpu.unbind_access_view_for_compute_shader( 0 );

        gpu.bind_shader_view_for_pixel_shader( shader_view, 0 );
        gpu.draw( screen_mesh );
        gpu.unbind_shader_view_for_pixel_shader( 0 );

        gpu.swap_buffers( true );
    }
    return 0;
}
