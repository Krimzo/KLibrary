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
    kl::Window window{ "Raytracing" };
    kl::GPU gpu{ window };
    kl::Timer timer;
    kl::Camera camera;

    window.on_resize.emplace_back([&](kl::Int2 size)
    {
        gpu.resize_internal(size);
        gpu.set_viewport_size(size);
        camera.update_aspect_ratio(size);
    });
    window.maximize();

    const std::string shader_sources = kl::read_file("shaders/raytracing.hlsl");
    kl::Shaders shaders = gpu.create_shaders(shader_sources);
    gpu.bind_shaders(shaders);

    const kl::dx::Buffer screen_mesh = gpu.create_screen_mesh();
    camera.position.y = 5.0f;

    struct alignas(16) CB
    {
        Sphere SPHERES[SPHERE_COUNT];
        kl::Float4x4 INVERSE_CAMERA;
        kl::Float3 CAMERA_POSITION;
        alignas(16) kl::Float3 SUN_DIRECTION;
    } cb = {};

    for (auto& sphere : cb.SPHERES) {
        sphere = Sphere{
            kl::random::gen_float3(40.0f) - kl::Float3(20.0f, 20.0f, 20.0f),
            kl::random::gen_float(2.75f) + 0.25f,
            kl::random::gen_rgb(),
        };
    }
    cb.SUN_DIRECTION = kl::normalize(kl::Float3(-1.0f, -1.0f, 0.0f));

    while (window.process()) {
        timer.update();

        for (int i = 0; i < SPHERE_COUNT; i++) {
            const float oscillation = (std::sin(timer.elapsed() + i) + 1.0f) * 0.5f;
            cb.SPHERES[i].center.y = (oscillation * (i + 1.0f)) + cb.SPHERES[i].radius;
        }
        
        if (window.keyboard.one.pressed()) {
            for (auto& sphere : cb.SPHERES) {
                sphere = Sphere{
                    kl::random::gen_float3(40.0f) - kl::Float3(20.0f, 20.0f, 20.0f),
                    kl::random::gen_float(2.75f) + 0.25f,
                    kl::random::gen_rgb(),
                };
            }
        }
        if (window.keyboard.two.pressed()) {
            for (auto& [center, radius, color] : cb.SPHERES) {
                color = kl::random::gen_rgb();
            }
        }
        if (window.keyboard.three.pressed()) {
            for (auto& [center, radius, color] : cb.SPHERES) {
                color = kl::random::gen_rgb(true);
            }
        }
        if (window.mouse.btn4) {
            const kl::Ray ray{
                camera.position,
                kl::inverse(camera.matrix()),
                window.mouse.norm_position(),
            };
            cb.SUN_DIRECTION = -ray.direction();
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

        cb.INVERSE_CAMERA = kl::inverse(camera.matrix());
        cb.CAMERA_POSITION = camera.position;

        shaders.upload(cb);
        gpu.draw(screen_mesh);

        gpu.swap_buffers(true);
    }
    return 0;
}
