#include "klib.h"


struct sky_ps_cb
{
    kl::float4 frame_size = {};
    kl::float4x4 inverse_camera = {};
    kl::float4 camera_position = {};
    kl::float4 sun_direction = {};
};

struct plane_vs_cb
{
    kl::float4x4 w_matrix = {};
    kl::float4x4 vp_matrix = {};
    kl::float4 time_data = {};
};

struct plane_ps_cb
{
    kl::float4 sun_direction = {};
};

static kl::timer timer = {};
static kl::camera camera = {};

static kl::float3 sun_direction = { 1, -1, 0 };

static kl::render_shaders plane_shaders = {};
static kl::dx::geometry_shader plane_geometry_shader = nullptr;

void setup_input(kl::window& window, kl::gpu& gpu);
void camera_movement(kl::window& window);

int main()
{
    kl::window window = { { 1600, 900 }, "Plane Playground" };
    kl::gpu gpu = { (HWND) window };

    setup_input(window, gpu);

    auto default_depth_state = gpu.create_depth_state(true, false, false);
    auto disabled_depth_state = gpu.create_depth_state(false, false, false);
    
    const std::string sky_shaders_source = kl::read_file_string("examples/shaders/sky.hlsl");
    auto sky_shaders = gpu.create_render_shaders(sky_shaders_source);

    auto screen_mesh = gpu.create_screen_mesh();
    auto plane_mesh = gpu.create_plane_mesh(10.0f, 1000);

    camera.origin = { -3.5f, 1.5f, -2.5f };
    camera.set_forward(camera.origin * -1.0f);

    while (window.process(false)) {
        timer.update_interval();

        camera_movement(window);

        gpu.clear_internal();

        // Sky
        gpu.bind_depth_state(disabled_depth_state);

        gpu.bind_render_shaders(sky_shaders);
        gpu.unbind_geometry_shader();

        sky_ps_cb sky_pscb = {};
        sky_pscb.frame_size = { kl::float2(window.size()), {} };
        sky_pscb.camera_position = { camera.origin, 0.0f };
        sky_pscb.inverse_camera = kl::inverse(camera.matrix());
        sky_pscb.sun_direction = { kl::normalize(sun_direction), 0.0f };
        sky_shaders.pixel_shader.update_cbuffer(sky_pscb);

        gpu.draw_vertex_buffer(screen_mesh);

        // Plane
        gpu.bind_depth_state(default_depth_state);

        gpu.bind_render_shaders(plane_shaders);
        gpu.bind_geometry_shader(plane_geometry_shader);

        plane_vs_cb plane_vscb = {};
        plane_vscb.w_matrix = {};
        plane_vscb.vp_matrix = camera.matrix();
        plane_vscb.time_data.x = timer.get_elapsed();
        plane_vscb.time_data.y = timer.get_interval();
        plane_shaders.vertex_shader.update_cbuffer(plane_vscb);

        plane_ps_cb plane_pscb = {};
        plane_pscb.sun_direction = { kl::normalize(sun_direction), 0 };
        plane_shaders.pixel_shader.update_cbuffer(plane_pscb);

        gpu.draw_vertex_buffer(plane_mesh);

        gpu.swap_buffers(true);
    }
}

void setup_input(kl::window& window, kl::gpu& gpu)
{
    window.on_resize.push_back([&](const kl::int2 new_size)
    {
        if (new_size.x > 0 && new_size.y > 0) {
            gpu.resize_internal(new_size);
            gpu.set_viewport_size(new_size);
            camera.update_aspect_ratio(new_size);
        }
    });
    window.maximize();

    window.keyboard.v.on_press.push_back([&]
    {
        static bool wireframe_bound = true;
        static auto solid_raster = gpu.create_raster_state(false, false, true);
        static auto wireframe_raster = gpu.create_raster_state(true, false, true);

        gpu.bind_raster_state(wireframe_bound ? solid_raster : wireframe_raster);
        wireframe_bound = !wireframe_bound;
    });
    window.keyboard.v.on_press.back()();

    window.keyboard.r.on_press.push_back([&]
    {
        kl::console::clear();

        const std::string shader_sources = kl::read_file_string("examples/shaders/playground.hlsl");
        const auto temp_default_shaders = gpu.create_render_shaders(shader_sources);
        const auto temp_geometry_shader = gpu.create_geometry_shader(shader_sources);

        if (temp_default_shaders && temp_geometry_shader) {
            plane_shaders = temp_default_shaders;
            plane_geometry_shader = temp_geometry_shader;
        }
    });
    window.keyboard.r.on_press.back()();

    window.mouse.left.on_down.push_back([&]
    {
        const kl::ray ray = { camera.origin, kl::inverse(camera.matrix()), window.mouse.get_normalized_position() };
        sun_direction = -ray.get_direction();
    });
}

void camera_movement(kl::window& window)
{
    static bool camera_rotating = false;

    if (window.mouse.right) {
        const kl::int2 frame_center = window.get_frame_center();

        if (camera_rotating) {
            camera.rotate(kl::float2(window.mouse.position()), kl::float2(frame_center));
        }
        window.mouse.set_position(frame_center);

        window.mouse.set_hidden(true);
        camera_rotating = true;
    }
    else if (camera_rotating) {
        window.mouse.set_hidden(false);
        camera_rotating = false;
    }

    if (window.keyboard.w) {
        camera.move_forward(timer.get_interval());
    }
    if (window.keyboard.s) {
        camera.move_back(timer.get_interval());
    }
    if (window.keyboard.d) {
        camera.move_right(timer.get_interval());
    }
    if (window.keyboard.a) {
        camera.move_left(timer.get_interval());
    }
    if (window.keyboard.e) {
        camera.move_up(timer.get_interval());
    }
    if (window.keyboard.q) {
        camera.move_down(timer.get_interval());
    }
}
