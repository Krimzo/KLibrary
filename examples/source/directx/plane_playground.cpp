#include "examples.h"


class SkyPsCb
{
public:
    kl::Float4 frame_size = {};
    kl::Float4x4 inverse_camera = {};
    kl::Float4 camera_position = {};
    kl::Float4 sun_direction = {};
};

class PlaneVsCb
{
public:
    kl::Float4x4 w_matrix = {};
    kl::Float4x4 vp_matrix = {};
    kl::Float4 time_data = {};
};

class PlanePsCb
{
public:
    kl::Float4 sun_direction = {};
};

static kl::Timer TIMER = {};
static kl::Camera CAMERA = {};

static kl::Float3 SUN_DIRECTION = { 1, -1, 0 };

static kl::RenderShaders PLANE_SHADERS = {};
static kl::dx::GeometryShader PLANE_GEOMETRY_SHADER = nullptr;

void setup_input(kl::Window& window, kl::GPU& gpu);
void camera_movement(kl::Window& window);

int examples::plane_playground_main()
{
    kl::Window window = { "Plane Playground", { 1600, 900 } };
    kl::GPU gpu = { (HWND) window };

    setup_input(window, gpu);

    const kl::dx::DepthState default_depth_state = gpu.create_depth_state(true, false, false);
    const kl::dx::DepthState disabled_depth_state = gpu.create_depth_state(false, false, false);
    
    const std::string sky_shaders_source = kl::read_file_string("shaders/sky.hlsl");
    kl::RenderShaders sky_shaders = gpu.create_render_shaders(sky_shaders_source);

    const kl::dx::Buffer screen_mesh = gpu.create_screen_mesh();
    const kl::dx::Buffer plane_mesh = gpu.create_plane_mesh(10.0f, 1000);

    CAMERA.origin = { -3.5f, 1.5f, -2.5f };
    CAMERA.set_forward(CAMERA.origin * -1.0f);

    while (window.process(false)) {
        TIMER.update_delta();

        camera_movement(window);

        gpu.clear_internal();

        // Sky
        gpu.bind_depth_state(disabled_depth_state);

        gpu.bind_render_shaders(sky_shaders);
        gpu.unbind_geometry_shader();

        SkyPsCb sky_pscb = {};
        sky_pscb.frame_size = { kl::Float2(window.size()), {} };
        sky_pscb.camera_position = { CAMERA.origin, 0.0f };
        sky_pscb.inverse_camera = kl::inverse(CAMERA.matrix());
        sky_pscb.sun_direction = { kl::normalize(SUN_DIRECTION), 0.0f };
        sky_shaders.pixel_shader.update_cbuffer(sky_pscb);

        gpu.draw(screen_mesh);

        // Plane
        gpu.bind_depth_state(default_depth_state);

        gpu.bind_render_shaders(PLANE_SHADERS);
        gpu.bind_geometry_shader(PLANE_GEOMETRY_SHADER);

        PlaneVsCb plane_vscb = {};
        plane_vscb.w_matrix = {};
        plane_vscb.vp_matrix = CAMERA.matrix();
        plane_vscb.time_data.x = TIMER.elapsed();
        plane_vscb.time_data.y = TIMER.delta();
        PLANE_SHADERS.vertex_shader.update_cbuffer(plane_vscb);

        PlanePsCb plane_pscb = {};
        plane_pscb.sun_direction = { kl::normalize(SUN_DIRECTION), 0 };
        PLANE_SHADERS.pixel_shader.update_cbuffer(plane_pscb);

        gpu.draw(plane_mesh);
        gpu.swap_buffers(true);
    }
    return 0;
}

void setup_input(kl::Window& window, kl::GPU& gpu)
{
    window.on_resize.emplace_back([&](const kl::Int2 new_size)
    {
        if (new_size.x > 0 && new_size.y > 0) {
            gpu.resize_internal(new_size);
            gpu.set_viewport_size(new_size);
            CAMERA.update_aspect_ratio(new_size);
        }
    });
    window.maximize();

    window.keyboard.v.on_press.emplace_back([&]
    {
        static bool wireframe_bound = true;
        static kl::dx::RasterState solid_raster = gpu.create_raster_state(false, false, true);
        static kl::dx::RasterState wireframe_raster = gpu.create_raster_state(true, false, true);

        gpu.bind_raster_state(wireframe_bound ? solid_raster : wireframe_raster);
        wireframe_bound = !wireframe_bound;
    });
    window.keyboard.v.on_press.back()();

    window.keyboard.r.on_press.emplace_back([&]
    {
        kl::console::clear();

        const std::string shader_sources = kl::read_file_string("shaders/playground.hlsl");
        const kl::RenderShaders temp_default_shaders = gpu.create_render_shaders(shader_sources);
        const kl::ShaderHolder temp_geometry_shader = gpu.create_geometry_shader(shader_sources);

        if (temp_default_shaders && temp_geometry_shader) {
            PLANE_SHADERS = temp_default_shaders;
            PLANE_GEOMETRY_SHADER = temp_geometry_shader;
        }
    });
    window.keyboard.r.on_press.back()();

    window.mouse.left.on_down.emplace_back([&]
    {
        const kl::Ray ray = { CAMERA.origin, kl::inverse(CAMERA.matrix()), window.mouse.normalized_position() };
        SUN_DIRECTION = -ray.direction();
    });
}

void camera_movement(kl::Window& window)
{
    static bool camera_rotating = false;

    if (window.mouse.right) {
        const kl::Int2 frame_center = window.frame_center();

        if (camera_rotating) {
            CAMERA.rotate(kl::Float2(window.mouse.position()), kl::Float2(frame_center));
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
        CAMERA.move_forward(TIMER.delta());
    }
    if (window.keyboard.s) {
        CAMERA.move_back(TIMER.delta());
    }
    if (window.keyboard.d) {
        CAMERA.move_right(TIMER.delta());
    }
    if (window.keyboard.a) {
        CAMERA.move_left(TIMER.delta());
    }
    if (window.keyboard.e) {
        CAMERA.move_up(TIMER.delta());
    }
    if (window.keyboard.q) {
        CAMERA.move_down(TIMER.delta());
    }
}
