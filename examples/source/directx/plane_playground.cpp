#include "klib.h"


/* Too tired to care, I'll update in future
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

static kl::ref<kl::window> window;
static kl::ref<kl::gpu> gpu;
static kl::timer timer;
static kl::camera camera;

static kl::shaders sky_shaders = {};
static kl::shaders plane_shaders = {};
static kl::dx::geometry_shader plane_geometry_shader = nullptr;

static kl::float3 sun_direction = { 1, -1, 0 };

static void setup_input()
{
    window->on_resize.push_back([&](const kl::int2 new_size)
    {
        if (new_size.x > 0 && new_size.y > 0) {
            gpu->resize_internal(new_size);
            gpu->set_viewport(new_size);
            camera.update_aspect_ratio(new_size);
        }
    });
    window->maximize();

    window->keyboard.v.on_press.push_back([&]
    {
        static bool wireframe_bound = true;
        static kl::dx::raster_state solid_raster = gpu->new_raster_state(false, false);
        static kl::dx::raster_state wireframe_raster = gpu->new_raster_state(true, false);
        if (wireframe_bound) {
            gpu->bind_raster_state(solid_raster);
        }
        else {
            gpu->bind_raster_state(wireframe_raster);
        }
        wireframe_bound = !wireframe_bound;
    });
    window->keyboard.v.on_press.back()();

    window->keyboard.r.on_press.push_back([&]
    {
        kl::console::clear();

        const std::string shader_source = kl::files::read_string("examples/shaders/playground.hlsl");
        const kl::shaders temp_default_shaders = gpu->new_shaders(shader_source);

        if (const kl::dx::geometry_shader temp_geometry_shader = gpu->new_geometry_shader(shader_source);
            temp_default_shaders && temp_geometry_shader) {
            gpu->destroy(plane_shaders);
            gpu->destroy(plane_geometry_shader);
            plane_shaders = temp_default_shaders;
            plane_geometry_shader = temp_geometry_shader;
        }
    });
    window->keyboard.r.on_press();

    window->mouse.right.on_down.push_back([&]
    {
        const kl::ray ray = { camera, window->mouse.get_normalized_position() };
        sun_direction = ray.direction.negate();
    });
}

static void camera_movement()
{
    static bool camera_rotating = false;
    if (window->mouse.left.state()) {
        const kl::int2 frame_center = window->get_frame_center();

        if (camera_rotating) {
            camera.rotate(kl::float2(window->mouse.position()), kl::float2(frame_center));
        }
        window->mouse.set_position(frame_center);

        window->mouse.set_hidden(true);
        camera_rotating = true;
    }
    else if (camera_rotating) {
        window->mouse.set_hidden(false);
        camera_rotating = false;
    }

    if (window->keyboard.w.state()) {
        camera.move_forward(timer.get_interval());
    }
    if (window->keyboard.s.state()) {
        camera.move_back(timer.get_interval());
    }
    if (window->keyboard.d.state()) {
        camera.move_right(timer.get_interval());
    }
    if (window->keyboard.a.state()) {
        camera.move_left(timer.get_interval());
    }
    if (window->keyboard.e.state()) {
        camera.move_up(timer.get_interval());
    }
    if (window->keyboard.q.state()) {
        camera.move_down(timer.get_interval());
    }
}

int main()
{
    window = kl::make<kl::window>(kl::int2(1600, 900), "Plane Playground");
    gpu = kl::make<kl::gpu>(window->get_window());

    setup_input();

    kl::dx::depth_state default_depth_state = gpu->new_depth_state(true, false, false);
    kl::dx::depth_state disabled_depth_state = gpu->new_depth_state(false, false, false);

    sky_shaders = gpu->new_shaders(kl::files::read_string("examples/shaders/sky.hlsl"));

    kl::dx::buffer screen_mesh = gpu->generate_screen_mesh();
    kl::dx::buffer plane_mesh = gpu->generate_plane_mesh(10.0f, 1000);

    camera.position = { -3.5f, 1.5f, -2.5f };
    camera.set_forward(camera.position.negate());

    while (window->process(false)) {
        timer.update_interval();

        camera_movement();

        gpu->clear_internal();

        // Sky
        gpu->bind_depth_state(disabled_depth_state);

        gpu->bind_shaders(sky_shaders);
        gpu->bind_geometry_shader(nullptr);

        sky_ps_cb sky_pscb = {};
        sky_pscb.frame_size = kl::float4(kl::float2(window->size()), kl::float2());
        sky_pscb.camera_position = kl::float4(camera.position, 0);
        sky_pscb.inverse_camera = camera.matrix().inverse();
        sky_pscb.sun_direction = { sun_direction.normalize(), 0 };
        gpu->set_pixel_const_buffer(sky_pscb);

        gpu->draw_vertex_buffer(screen_mesh);

        // Plane
        gpu->bind_depth_state(default_depth_state);

        gpu->bind_shaders(plane_shaders);
        gpu->bind_geometry_shader(plane_geometry_shader);

        plane_vs_cb plane_vscb = {};
        plane_vscb.w_matrix = kl::mat4();
        plane_vscb.vp_matrix = camera.matrix();
        plane_vscb.time_data.x = timer.get_elapsed();
        plane_vscb.time_data.y = timer.get_interval();
        gpu->set_vertex_const_buffer(plane_vscb);

        plane_ps_cb plane_pscb = {};
        plane_pscb.sun_direction = { sun_direction.normalize(), 0 };
        gpu->set_pixel_const_buffer(plane_pscb);

        gpu->draw_vertex_buffer(plane_mesh);


        gpu->swap_buffers(true);
    }
}
*/
