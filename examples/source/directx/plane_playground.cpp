#include "examples.h"


static kl::Timer TIMER;
static kl::Camera CAMERA;

static kl::Float3 SUN_DIRECTION = { 1, -1, 0 };

static kl::Shaders PLANE_SHADERS;
static kl::dx::GeometryShader PLANE_GEOMETRY_SHADER;

void camera_movement(kl::Window& window);

int examples::plane_playground_main(const int argc, const char** argv)
{
    kl::Window window{ "Plane Playground" };
    kl::GPU gpu{ window.ptr() };

    window.on_resize.emplace_back([&](kl::Int2 size)
    {
        gpu.resize_internal(size);
        gpu.set_viewport_size(size);
        CAMERA.update_aspect_ratio(size);
    });
    window.maximize();

    const kl::dx::DepthState default_depth_state = gpu.create_depth_state(true, false, false);
    const kl::dx::DepthState disabled_depth_state = gpu.create_depth_state(false, false, false);
    
    const std::string sky_shaders_source = kl::read_file("shaders/sky.hlsl");
    kl::Shaders sky_shaders = gpu.create_shaders(sky_shaders_source);

    const kl::dx::Buffer screen_mesh = gpu.create_screen_mesh();
    const kl::dx::Buffer plane_mesh = gpu.create_plane_mesh(10.0f, 1000);

    CAMERA.position = { -3.5f, 1.5f, -2.5f };
    CAMERA.set_forward(CAMERA.position * -1.0f);

    int frame_id = 0;
    while (window.process()) {
        frame_id += 1;
        TIMER.update();

        if (frame_id == 1 || window.keyboard.v.pressed()) {
            static bool wireframe_bound = true;
            static kl::dx::RasterState solid_raster = gpu.create_raster_state(false, false, true);
            static kl::dx::RasterState wireframe_raster = gpu.create_raster_state(true, false, true);

            gpu.bind_raster_state(wireframe_bound ? solid_raster : wireframe_raster);
            wireframe_bound = !wireframe_bound;
        }
        if (frame_id == 1 || window.keyboard.r.pressed()) {
            kl::console::clear();

            std::string shader_sources = kl::read_file("shaders/playground.hlsl");
            kl::Shaders temp_default_shaders = gpu.create_shaders(shader_sources);
            kl::GeometryShader temp_geometry_shader = gpu.create_geometry_shader(shader_sources);

            if (temp_default_shaders && temp_geometry_shader) {
                PLANE_SHADERS = temp_default_shaders;
                PLANE_GEOMETRY_SHADER = temp_geometry_shader.shader;
            }
        }
        if (window.mouse.btn4) {
            const kl::Ray ray{
                CAMERA.position,
                kl::inverse(CAMERA.matrix()),
                window.mouse.norm_position(),
            };
            SUN_DIRECTION = -ray.direction();
        }

        camera_movement(window);

        gpu.clear_internal();

        gpu.bind_depth_state(disabled_depth_state);
        gpu.bind_shaders(sky_shaders);
        gpu.unbind_geometry_shader();

        struct alignas(16) SKY_CB
        {
            kl::Float4x4 INVERSE_CAMERA;
            kl::Float4 FRAME_SIZE;
            kl::Float4 SUN_DIRECTION;
        } sky_cb = {};

        sky_cb.INVERSE_CAMERA = kl::inverse(CAMERA.matrix());
        sky_cb.FRAME_SIZE = { kl::Float2(window.size()), {} };
        sky_cb.SUN_DIRECTION = { kl::normalize(SUN_DIRECTION), 0.0f };

        sky_shaders.upload(sky_cb);
        gpu.draw(screen_mesh);

        if (!PLANE_SHADERS || !PLANE_GEOMETRY_SHADER)
            continue;

        gpu.bind_depth_state(default_depth_state);
        gpu.bind_shaders(PLANE_SHADERS);
        gpu.bind_geometry_shader(PLANE_GEOMETRY_SHADER);

        struct alignas(16) PLANE_CB
        {
            kl::Float4x4 W;
            kl::Float4x4 VP;
            kl::Float4 TIME_DATA;
            kl::Float4 SUN_DIRECTION;
        } plane_cb = {};

        plane_cb.W = {};
        plane_cb.VP = CAMERA.matrix();
        plane_cb.TIME_DATA = { TIMER.elapsed(), TIMER.delta(), 0.0f, 0.0f };
        plane_cb.SUN_DIRECTION = { kl::normalize(SUN_DIRECTION), 0.0f };

        PLANE_SHADERS.upload(plane_cb);
        gpu.draw(plane_mesh);

        gpu.swap_buffers(true);
    }
    return 0;
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
