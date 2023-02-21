#include "klib.h"


struct cs_cb
{
    kl::float4 misc_data;
};

int main()
{
    kl::window window = { { 1600, 900 }, "Compute shaders 1" };
    kl::gpu gpu = { (HWND) window };
    kl::timer timer = {};

    window.set_resizeable(false);
    gpu.resize_to_window(window);

    auto depth_state = gpu.create_depth_state(false, false, false);
    gpu.bind_depth_state(depth_state);

    auto render_texture = gpu.create_texture({ kl::int2(1600, 900) }, true, true);
    auto target_view = gpu.create_target_view(render_texture, nullptr);
    auto access_view = gpu.create_access_view(render_texture, nullptr);
    auto shader_view = gpu.create_shader_view(render_texture, nullptr);

    const std::string shader_sources = kl::files::read_string("examples/shaders/compute_test1.hlsl");
    auto default_shaders = gpu.create_render_shaders(shader_sources);
    auto compute_shader = gpu.create_compute_shader(shader_sources);
    gpu.bind_render_shaders(default_shaders);
    gpu.bind_compute_shader(compute_shader);

    auto screen_mesh = gpu.create_screen_mesh();

    auto cs_const_buffer = gpu.create_const_buffer(sizeof(cs_cb));
    gpu.bind_cb_for_compute_shader(cs_const_buffer, 0);

    while (window.process(false)) {
        timer.update_interval();

        gpu.unbind_shader_view_for_compute_shader(0);
        gpu.unbind_shader_view_for_pixel_shader(0);
        gpu.clear_target_view(target_view, kl::colors::gray);

        cs_cb cs_data = {};
        cs_data.misc_data.x = (float) window.mouse.position().x;
        cs_data.misc_data.y = (float) window.mouse.position().y;
        gpu.set_cb_data(cs_const_buffer, cs_data);

        gpu.unbind_shader_view_for_pixel_shader(0);
        gpu.bind_access_view_for_compute_shader(access_view, 0);
        gpu.dispatch_compute_shader(
            (window.width() / 32) + ((window.width() % 32) ? 1 : 0),
            (window.height() / 32) + ((window.height() % 32) ? 1 : 0),
            1
        );

        gpu.unbind_access_view_for_compute_shader(0);
        gpu.bind_shader_view_for_pixel_shader(shader_view, 0);
        gpu.draw_mesh(screen_mesh);

        gpu.swap_buffers(true);
    }
}
