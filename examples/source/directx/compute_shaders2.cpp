#include "examples.h"


struct compute2_cs_cb
{
    kl::float4 misc_data;
};

int examples::compute_shaders2_main()
{
    kl::window window = { "Compute shaders 2", { 1600, 900 } };
    kl::gpu gpu = { (HWND) window };
    kl::timer timer = {};

    window.set_resizeable(false);
    gpu.resize_to_window(window);

    kl::dx::depth_state depth_state = gpu.create_depth_state(false, false, false);
    gpu.bind_depth_state(depth_state);

    kl::dx::texture render_texture = gpu.create_texture(kl::image{ kl::int2{ 1600, 900 } }, true, true);
    kl::dx::target_view target_view = gpu.create_target_view(render_texture, nullptr);
    kl::dx::access_view access_view = gpu.create_access_view(render_texture, nullptr);
    kl::dx::shader_view shader_view = gpu.create_shader_view(render_texture, nullptr);

    const std::string shader_sources = kl::read_file_string("shaders/compute_test2.hlsl");
    kl::render_shaders render_shaders = gpu.create_render_shaders(shader_sources);
    kl::shader_holder compute_shader = gpu.create_compute_shader(shader_sources);
    gpu.bind_render_shaders(render_shaders);
    gpu.bind_compute_shader(compute_shader);

    kl::dx::buffer screen_mesh = gpu.create_screen_mesh();

    while (window.process(false)) {
        timer.update_delta();

        gpu.unbind_shader_view_for_compute_shader(0);
        gpu.unbind_shader_view_for_pixel_shader(0);
        gpu.clear_target_view(target_view, kl::colors::gray);

        compute2_cs_cb cs_data = {};
        cs_data.misc_data.x = (float) window.mouse.position().x;
        cs_data.misc_data.y = (float) window.mouse.position().y;
        compute_shader.update_cbuffer(cs_data);

        gpu.unbind_shader_view_for_pixel_shader(0);
        gpu.bind_access_view_for_compute_shader(access_view, 0);
        gpu.dispatch_compute_shader((window.width() / 32) + 1, (window.height() / 32) + 1, 1);

        gpu.unbind_access_view_for_compute_shader(0);
        gpu.bind_shader_view_for_pixel_shader(shader_view, 0);
        gpu.draw(screen_mesh);

        gpu.swap_buffers(true);
    }
    return 0;
}
