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

static kl::ref<kl::gpu_shaders> plane_shaders = nullptr;
static kl::ref<kl::gpu_geometry_shader> plane_geometry_shader = nullptr;

void setup_input(kl::ref<kl::window> window, kl::ref<kl::gpu> gpu);
void camera_movement(kl::ref<kl::window> window);

int main()
{
    kl::BOUND_WINDOW = kl::window::make({ 1600, 900 }, "Plane Playground");
    kl::BOUND_GPU = kl::gpu::make(*kl::BOUND_WINDOW);

    auto& window = kl::BOUND_WINDOW;
    auto& gpu = kl::BOUND_GPU;

    auto raster_state = kl::gpu_raster_state::make(false, false, true);
    raster_state->bind();

    setup_input(window, gpu);

    auto default_depth_state = kl::gpu_depth_state::make(true, false, false);
    auto disabled_depth_state = kl::gpu_depth_state::make(false, false, false);

    auto sky_shaders = kl::gpu_shaders::make(kl::files::read_string("examples/shaders/sky.hlsl"));

    auto screen_mesh = kl::gpu_mesh::make_screen();
    auto plane_mesh = kl::gpu_mesh::make_plane(10.0f, 1000);

    auto sky_ps_const_buffer = kl::gpu_const_buffer::make(sizeof(sky_ps_cb));
    auto plane_vs_const_buffer = kl::gpu_const_buffer::make(sizeof(plane_vs_cb));
    auto plane_ps_const_buffer = kl::gpu_const_buffer::make(sizeof(plane_ps_cb));

    camera.position = { -3.5f, 1.5f, -2.5f };
    camera.set_forward(camera.position * -1.0f);

    while (window->process(false)) {
        timer.update_interval();

        camera_movement(window);

        gpu->clear_internal();

        // Sky
        disabled_depth_state->bind();

        sky_shaders->bind();
        kl::gpu_geometry_shader::unbind();

        sky_ps_cb sky_pscb = {};
        sky_pscb.frame_size = { kl::float2(window->size()), {} };
        sky_pscb.camera_position = { camera.position, 0.0f };
        sky_pscb.inverse_camera = kl::math::inverse(camera.matrix());
        sky_pscb.sun_direction = { kl::math::normalize(sun_direction), 0.0f };

        sky_ps_const_buffer->bind_for_pixel_shader(0);
        sky_ps_const_buffer->set_data(sky_pscb);

        screen_mesh->draw();

        // Plane
        default_depth_state->bind();

        plane_shaders->bind();
        plane_geometry_shader->bind();

        plane_vs_cb plane_vscb = {};
        plane_vscb.w_matrix = {};
        plane_vscb.vp_matrix = camera.matrix();
        plane_vscb.time_data.x = timer.get_elapsed();
        plane_vscb.time_data.y = timer.get_interval();
        
        plane_vs_const_buffer->bind_for_vertex_shader(0);
        plane_vs_const_buffer->set_data(plane_vscb);

        plane_ps_cb plane_pscb = {};
        plane_pscb.sun_direction = { kl::math::normalize(sun_direction), 0 };
        
        plane_ps_const_buffer->bind_for_pixel_shader(0);
        plane_ps_const_buffer->set_data(plane_pscb);

        plane_mesh->draw();


        gpu->swap_buffers(true);
    }
}

void setup_input(kl::ref<kl::window> window, kl::ref<kl::gpu> gpu)
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
        static auto solid_raster = kl::gpu_raster_state::make(false, false, true);
        static auto wireframe_raster = kl::gpu_raster_state::make(true, false, true);

        if (wireframe_bound) {
            solid_raster->bind();
        }
        else {
            wireframe_raster->bind();
        }
        wireframe_bound = !wireframe_bound;
    });
    window->keyboard.v.on_press.back()();

    window->keyboard.r.on_press.push_back([&]
    {
        kl::console::clear();

        const std::string shader_source = kl::files::read_string("examples/shaders/playground.hlsl");
        const auto temp_default_shaders = kl::gpu_shaders::make(shader_source);
        const auto temp_geometry_shader = kl::gpu_geometry_shader::make(shader_source);

        if (temp_default_shaders && temp_geometry_shader) {
            plane_shaders = temp_default_shaders;
            plane_geometry_shader = temp_geometry_shader;
        }
     });
    window->keyboard.r.on_press.back()();

    window->mouse.right.on_down.push_back([&]
    {
        const kl::ray ray = { camera.position, kl::math::inverse(camera.matrix()), window->mouse.get_normalized_position()};
        sun_direction = ray.direction * -1.0f;
    });
}

void camera_movement(kl::ref<kl::window> window)
{
    static bool camera_rotating = false;

    if (window->mouse.left) {
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

    if (window->keyboard.w) {
        camera.move_forward(timer.get_interval());
    }
    if (window->keyboard.s) {
        camera.move_back(timer.get_interval());
    }
    if (window->keyboard.d) {
        camera.move_right(timer.get_interval());
    }
    if (window->keyboard.a) {
        camera.move_left(timer.get_interval());
    }
    if (window->keyboard.e) {
        camera.move_up(timer.get_interval());
    }
    if (window->keyboard.q) {
        camera.move_down(timer.get_interval());
    }
}
