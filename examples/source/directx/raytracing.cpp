#include "examples.h"


static constexpr int SPHERE_COUNT = 10;

struct Sphere
{
    kl::Float3 center;
    float radius = 0.0f;
    kl::Float4 color;
};

int examples::raytracing_main(const int argc, const char** argv)
{
    kl::Window window = { "Raytracing", { 1600, 900 } };
    kl::GPU gpu = { HWND(window) };
    kl::Timer timer;
    kl::Camera camera;

    window.on_resize.emplace_back([&](const kl::Int2 new_size)
    {
        if (new_size.x > 0 && new_size.y > 0) {
            gpu.resize_internal(new_size);
            gpu.set_viewport_size(new_size);
            camera.update_aspect_ratio(new_size);
        }
    });
    window.maximize();

    const std::string shader_sources = kl::read_file("shaders/raytracing.hlsl");
    kl::RenderShaders shaders = gpu.create_render_shaders(shader_sources);
    gpu.bind_render_shaders(shaders);

    const kl::dx::Buffer screen_mesh = gpu.create_screen_mesh();
    camera.position.y = 5.0f;

    struct alignas(16) CB
    {
        Sphere SPHERES[SPHERE_COUNT];
        kl::Float4x4 INVERSE_CAMERA;
        kl::Float4 FRAME_SIZE;
        kl::Float4 CAMERA_POSITION;
        kl::Float4 SUN_DIRECTION;
    };
    kl::Ref<CB> cb = new CB();

    cb->SUN_DIRECTION = { kl::normalize(kl::Float3(-1.0f, -1.0f, 0.0f)), 0.0f };

    for (auto& sphere : cb->SPHERES) {
        sphere = Sphere{
            kl::random::gen_float3(40.0f) - kl::Float3(20.0f, 20.0f, 20.0f),
            kl::random::gen_float(2.75f) + 0.25f,
            kl::random::gen_color(),
        };
    }

    while (window.process()) {
        timer.update_delta();

        for (int i = 0; i < SPHERE_COUNT; i++) {
            const float oscillation = (std::sin(timer.elapsed() + i) + 1.0f) * 0.5f;
            cb->SPHERES[i].center.y = (oscillation * (i + 1.0f)) + cb->SPHERES[i].radius;
        }
        
        if (window.keyboard.num1.pressed()) {
            for (auto& sphere : cb->SPHERES) {
                sphere = Sphere{
                    kl::random::gen_float3(40.0f) - kl::Float3(20.0f, 20.0f, 20.0f),
                    kl::random::gen_float(2.75f) + 0.25f,
                    kl::random::gen_color(),
                };
            }
        }
        if (window.keyboard.num2.pressed()) {
            for (auto& [center, radius, color] : cb->SPHERES) {
                color = kl::random::gen_color();
            }
        }
        if (window.keyboard.num3.pressed()) {
            for (auto& [center, radius, color] : cb->SPHERES) {
                color = kl::random::gen_color(true);
            }
        }
        if (window.mouse.btn4) {
            const kl::Ray ray{
                camera.position,
                kl::inverse(camera.matrix()),
                window.mouse.norm_position(),
            };
            cb->SUN_DIRECTION = { -ray.direction(), 0.0f };
        }

        static bool camera_rotating = false;
        if (window.mouse.right) {
            const kl::Int2 frame_center = window.frame_center();

            if (camera_rotating) {
                camera.rotate(window.mouse.position(), frame_center);
            }
            window.mouse.set_position(frame_center);

            window.mouse.set_hidden(true);
            camera_rotating = true;
        }
        else if (camera_rotating) {
            window.mouse.set_hidden(false);
            camera_rotating = false;
        }

        if (window.keyboard.shift) {
            camera.speed = 5.0f;
        }
        else {
            camera.speed = 2.0f;
        }

        if (window.keyboard.w) {
            camera.move_forward(timer.delta());
        }
        if (window.keyboard.s) {
            camera.move_back(timer.delta());
        }
        if (window.keyboard.d) {
            camera.move_right(timer.delta());
        }
        if (window.keyboard.a) {
            camera.move_left(timer.delta());
        }
        if (window.keyboard.e) {
            camera.move_up(timer.delta());
        }
        if (window.keyboard.q) {
            camera.move_down(timer.delta());
        }

        gpu.clear_internal();

        cb->FRAME_SIZE = { window.size(), {} };
        cb->INVERSE_CAMERA = kl::inverse(camera.matrix());
        cb->CAMERA_POSITION = { camera.position, {} };

        shaders.upload<CB>(*cb);
        gpu.draw(screen_mesh);

        gpu.swap_buffers(true);
    }
    return 0;
}
