#include "klib.h"


struct vs_cb
{
	kl::mat4 w_matrix;
	kl::mat4 vp_matrix;
	kl::float4 misc_data;
};

struct ps_cb
{
	kl::float4 object_color;
	kl::float4 sun_direction;
};

static kl::ref<kl::window> window;
static kl::ref<kl::gpu> gpu;
static kl::timer timer;
static kl::camera camera;

static kl::float3 sun_direction = { 1.0f, -1.0f, 0.0f };

static std::vector<kl::ref<kl::entity>> entities;

int main()
{
	window = kl::make<kl::window>(kl::uint2(1600, 900), "Geometry Test");
	gpu = kl::make<kl::gpu>(window->get_window());

	window->on_resize = [&](kl::uint2 new_size) {
		if (new_size.x > 0 && new_size.y > 0) {
			gpu->resize_internal(new_size);
			gpu->set_viewport(new_size);
			camera.update_aspect_ratio(new_size);
		}
	};
	window->maximize();

	window->keyboard.v.on_press = [&] {
		static bool wireframeBound = true;
		static kl::dx::raster_state solidRaster = gpu->new_raster_state(false, false);
		static kl::dx::raster_state wireframeRaster = gpu->new_raster_state(true, false);
		if (wireframeBound) {
			gpu->bind_raster_state(solidRaster);
		}
		else {
			gpu->bind_raster_state(wireframeRaster);
		}
		wireframeBound = !wireframeBound;
	};
	window->keyboard.v.on_press();

	std::string shaders_source = kl::files::read_string("examples/shaders/geometry_test.hlsl");
	kl::shaders default_shaders = gpu->new_shaders(shaders_source);
	kl::dx::geometry_shader geometry_shader = gpu->new_geometry_shader(shaders_source);

	gpu->bind_shaders(default_shaders);
	gpu->bind_geometry_shader(geometry_shader);

	kl::dx::buffer cube_mesh = gpu->new_vertex_buffer("examples/meshes/cube.obj", true);
	kl::dx::buffer sphere_mesh = gpu->new_vertex_buffer("examples/meshes/sphere.obj", true);
	kl::dx::buffer monke_mesh = gpu->new_vertex_buffer("examples/meshes/monke.obj", true);

	kl::ref<kl::entity> main_entity = kl::make<kl::entity>();
	main_entity->angular.y = -36.0f;
	main_entity->mesh = monke_mesh;
	main_entity->color = kl::float4(kl::colors::orange);
	entities.push_back(main_entity);

	window->keyboard.num1.on_press = [&] {
		main_entity->mesh = cube_mesh;
	};
	window->keyboard.num2.on_press = [&] {
		main_entity->mesh = sphere_mesh;
	};
	window->keyboard.num3.on_press = [&] {
		main_entity->mesh = monke_mesh;
	};

	float destroy_goal = 0.0f;
	float destroy_value = 0.0f;
	window->keyboard.space.on_press = [&] {
		destroy_goal = 1.5f;
	};

	camera.position = { -2.0f, 2.0f, -2.0f };
	camera.set_forward(camera.position.negate());

	while (window->process(false)) {
		timer.update_interval();

		for (auto& entity : entities) {
			entity->update_physics(timer.get_interval());
		}

		if (destroy_value < destroy_goal) {
			destroy_value += timer.get_interval() * 5.0f;
		}
		else {
			destroy_goal = -INFINITY;
			destroy_value -= timer.get_interval() * 0.25f;
		}

		gpu->clear_internal();

		vs_cb vscb = {};
		vscb.vp_matrix = camera.matrix();

		ps_cb pscb = {};
		pscb.sun_direction = { sun_direction.normalize(), 0.0f };

		for (auto& entity : entities) {
			if (entity) {
				vscb.w_matrix = entity->matrix();
				vscb.misc_data.x = std::max(destroy_value, 0.0f);
				gpu->set_vertex_const_buffer(vscb);

				pscb.object_color = entity->color;
				gpu->set_pixel_const_buffer(pscb);

				if (entity->mesh) {
					gpu->draw_vertex_buffer(entity->mesh);
				}
			}
		}

		gpu->swap_buffers(true);
	}
}
