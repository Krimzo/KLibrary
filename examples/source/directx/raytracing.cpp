#include "examples.h"


static constexpr int SPHERE_COUNT = 10;

int examples::raytracing_main()
{
    struct ColoredSphere
    {
        kl::Float3 center = {};
        float radius = 0.0f;
        kl::Float4 color = {};
    };

    struct PSData
    {
        kl::Float4 frame_size = {};
        kl::Float4x4 inverse_camera = {};
        kl::Float4 camera_position = {};
        kl::Float4 sun_direction = {};
        ColoredSphere spheres[SPHERE_COUNT] = {};
    };

    kl::Timer timer = {};
    kl::Camera camera = {};

    kl::Window window = { "Raytracing", { 1600, 900 } };
    kl::GPU gpu = { static_cast<HWND>(window) };

    // Heap alloc because of stack size warnings
    PSData& ps_data = *new PSData; // You saw nothing :)

    window.on_resize.emplace_back([&](const kl::Int2 new_size)
    {
        if (new_size.x > 0 && new_size.y > 0) {
            gpu.resize_internal(new_size);
            gpu.set_viewport_size(new_size);
            camera.update_aspect_ratio(new_size);
        }
    });

    window.keyboard.r.on_press.emplace_back([&]
    {
        if (window.keyboard.shift) {
            for (auto& [center, radius, color] : ps_data.spheres) {
                color = (kl::Float4) kl::random::gen_color();
            }
        }
        else if (window.keyboard.ctrl) {
            for (auto& [center, radius, color] : ps_data.spheres) {
                color = (kl::Float4) kl::Color(color).gray();
            }
        }
        else {
            for (auto& sphere : ps_data.spheres) {
                sphere = ColoredSphere {
                    kl::random::gen_float3(40.0f) - kl::Float3(20.0f, 20.0f, 20.0f),
                    kl::random::gen_float(2.75f) + 0.25f,
                    (kl::Float4) kl::random::gen_color(),
                };
            }
        }
    });

    window.mouse.left.on_down.emplace_back([&]
    {
        const kl::Ray ray = { camera.origin, kl::inverse(camera.matrix()), window.mouse.normalized_position() };
        ps_data.sun_direction = { ray.direction() * -1.0f, 0.0f };
    });

    // Start
    window.maximize();

    const std::string shader_sources = kl::read_file_string("shaders/raytracing.hlsl");
    kl::RenderShaders shaders = gpu.create_render_shaders(shader_sources);
    gpu.bind_render_shaders(shaders);

    const kl::dx::Buffer screen_mesh = gpu.create_screen_mesh();

    camera.origin.y = 5.0f;
    ps_data.sun_direction = { kl::normalize(kl::Float3(-1.0f, -1.0f, 0.0f)), 0.0f };

    window.keyboard.r.on_press.back()();

    // Update
    while (window.process(false)) {
        timer.update_delta();

        { // Physics
            for (int i = 0; i < SPHERE_COUNT; i++) {
                const float oscillation = (std::sin(timer.elapsed() + i) + 1.0f) * 0.5f;
                ps_data.spheres[i].center.y = (oscillation * (i + 1.0f)) + ps_data.spheres[i].radius;
            }
        }

        { // Input
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
        }

        // Render
        gpu.clear_internal();

        ps_data.frame_size = { window.size(), {} };
        ps_data.inverse_camera = kl::inverse(camera.matrix());
        ps_data.camera_position = { camera.origin, {} };
        shaders.pixel_shader.update_cbuffer(ps_data);

        gpu.draw(screen_mesh);

        gpu.swap_buffers(true);
    }
    return 0;
}
