#include "klib.h"


struct cs_cb
{
	kl::float4 misc_data;
};

static kl::uint3 calculate_window_dispatch(const kl::window& window)
{
	return {
		(window.width() / 32) + ((window.width() % 32) ? 1 : 0),
		(window.height() / 32) + ((window.height() % 32) ? 1 : 0),
		1
	};
}

int main()
{
	kl::window window = { { 1600, 900 }, "Testing" };
	auto gpu = kl::gpu(window.get_window());
	kl::timer timer = {};

	window.set_resizeable(false);

	gpu.bind_depth_state(gpu.new_depth_state(false, false, false));

	kl::dx::texture render_texture = gpu.new_texture(kl::image({1600, 900}, kl::colors::gray), true, true);
	kl::dx::shader_view pixel_shader_view = gpu.new_shader_view(render_texture);
	kl::dx::access_view compute_shader_view = gpu.new_access_view(render_texture);
	kl::dx::target_view target_view = gpu.new_target_view(render_texture);

	std::string shaders_source = kl::files::read_string("examples/shaders/compute_test1.hlsl");
	kl::shaders default_shaders = gpu.new_shaders(shaders_source);
	kl::dx::compute_shader compute_shader = gpu.new_compute_shader(shaders_source);

	gpu.bind_shaders(default_shaders);
	gpu.bind_compute_shader(compute_shader);

	kl::dx::buffer screen_mesh = gpu.generate_screen_mesh();

	while (window.process(false)) {
		timer.update_interval();

		gpu.bind_compute_shader_view(nullptr, 0);
		gpu.bind_pixel_shader_view(nullptr, 0);
		gpu.clear_target_view(target_view, kl::float4(kl::colors::gray));

		cs_cb cs_data = {};
		cs_data.misc_data.x = static_cast<float>(window.mouse.position().x);
		cs_data.misc_data.y = static_cast<float>(window.mouse.position().y);
		gpu.set_compute_const_buffer(cs_data);

		gpu.bind_pixel_shader_view(nullptr, 0);
		gpu.bind_compute_access_view(compute_shader_view, 0);
		gpu.dispatch_compute_shader(calculate_window_dispatch(window));

		gpu.bind_compute_access_view(nullptr, 0);
		gpu.bind_pixel_shader_view(pixel_shader_view, 0);
		gpu.draw_vertex_buffer(screen_mesh);

		gpu.swap_buffers(true);

		window.set_title(kl::format(static_cast<int>(1.0f / timer.get_interval())));
	}
}
