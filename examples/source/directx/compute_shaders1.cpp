#include "klib.h"


struct cs_cb
{
    kl::float4 misc_data;
};

int main()
{
    kl::BOUND_WINDOW = kl::window::make({ 1600, 900 }, "Testing");
    kl::BOUND_GPU = kl::gpu::make(*kl::BOUND_WINDOW);
    kl::timer timer = {};

    auto& window = *kl::BOUND_WINDOW;
    auto& gpu = *kl::BOUND_GPU;

    window.set_resizeable(false);
    gpu.resize_to_window(*kl::BOUND_WINDOW);

    auto raster_state = kl::gpu_raster_state::make(false, false, true);
    raster_state->bind();

    auto depth_state = kl::gpu_depth_state::make(false, false, false);
    depth_state->bind();

    auto xD = kl::image(kl::int2(1600, 900));
    xD.fill(kl::colors::orange);

    auto render_texture = kl::gpu_texture::make(xD, true, true);
    auto target_view = kl::gpu_target_view::make(*render_texture, nullptr);
    auto access_view = kl::gpu_access_view::make(*render_texture, nullptr);
    auto shader_view = kl::gpu_shader_view::make(*render_texture, nullptr);

    std::string shaders_source = kl::files::read_string("examples/shaders/compute_test1.hlsl");
    auto default_shaders = kl::gpu_shaders::make(shaders_source);
    auto compute_shader = kl::gpu_compute_shader::make(shaders_source);
    default_shaders->bind();
    compute_shader->bind();

    auto screen_mesh = kl::gpu_mesh::make_screen();

    auto cs_const_buffer = kl::gpu_const_buffer::make(sizeof(cs_cb));
    cs_const_buffer->bind_for_compute_shader(0);

    while (window.process(false)) {
        timer.update_interval();

        kl::gpu_shader_view::unbind_for_compute_shader(0);
        kl::gpu_shader_view::unbind_for_pixel_shader(0);
        target_view->clear(kl::colors::gray);

        cs_cb cs_data = {};
        cs_data.misc_data.x = (float) window.mouse.position().x;
        cs_data.misc_data.y = (float) window.mouse.position().y;
        cs_const_buffer->set_data(cs_data);

        kl::gpu_shader_view::unbind_for_pixel_shader(0);
        access_view->bind(0);
        compute_shader->dispatch((window.width() / 32) + ((window.width() % 32) ? 1 : 0), (window.height() / 32) + ((window.height() % 32) ? 1 : 0), 1);

        kl::gpu_access_view::unbind(0);
        shader_view->bind_for_pixel_shader(0);
        screen_mesh->draw();

        gpu.swap_buffers(true);

        window.set_title(kl::format((int) (1.0f / timer.get_interval())));
    }
}
