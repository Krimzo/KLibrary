#include "examples.h"


int examples::geometry_shaders_main()
{
    /* ----- SETUP ----- */
    kl::Window window = { "Geometry Test", { 1600, 900 } };
    kl::GPU gpu = { static_cast<HWND>(window) };

    kl::Timer timer = {};
    kl::Camera camera = {};

    kl::Float3 sun_direction = { 1.0f, -1.0f, 0.0f };

    // Window
    window.on_resize.emplace_back([&](const kl::Int2 new_size)
    {
        if (new_size.x > 0 && new_size.y > 0) {
            gpu.resize_internal(new_size);
            gpu.set_viewport_size(new_size);
            camera.update_aspect_ratio(new_size);
        }
    });
    window.maximize();

    window.keyboard.v.on_press.emplace_back([&]
    {
        static bool wireframe_bound = true;
        static kl::dx::RasterState solid_raster = gpu.create_raster_state(false, false);
        static kl::dx::RasterState wireframe_raster = gpu.create_raster_state(true, false);

        gpu.bind_raster_state(wireframe_bound ? solid_raster : wireframe_raster);
        wireframe_bound = !wireframe_bound;
    });
    window.keyboard.v.on_press.back()();

    // Shaders
    std::string shader_sources = kl::read_file_string("shaders/geometry_test.hlsl");
    kl::RenderShaders default_shaders = gpu.create_render_shaders(shader_sources);
    kl::ShaderHolder geometry_shader = gpu.create_geometry_shader(shader_sources);
    gpu.bind_render_shaders(default_shaders);
    gpu.bind_geometry_shader(geometry_shader);

    // Mesh
    kl::Object cube_mesh = new kl::Mesh(&gpu);
    kl::Object sphere_mesh = new kl::Mesh(&gpu);
    kl::Object monke_mesh = new kl::Mesh(&gpu);
    cube_mesh->graphics_buffer = gpu.create_vertex_buffer("meshes/cube.obj");
    sphere_mesh->graphics_buffer =gpu.create_vertex_buffer("meshes/sphere.obj");
    monke_mesh->graphics_buffer =gpu.create_vertex_buffer("meshes/monke.obj");

    // Material
    kl::Object default_material = new kl::Material();
    default_material->color = kl::colors::ORANGE;

    // Entity
    kl::Object main_entity = new kl::Entity();
    main_entity->angular.y = -36.0f;
    main_entity->mesh = monke_mesh;
    main_entity->material = default_material;

    // Input
    window.keyboard.num1.on_press.emplace_back([&]
    {
        main_entity->mesh = cube_mesh;
    });
    window.keyboard.num2.on_press.emplace_back([&]
    {
        main_entity->mesh = sphere_mesh;
    });
    window.keyboard.num3.on_press.emplace_back([&]
    {
        main_entity->mesh = monke_mesh;
    });

    float destroy_goal = 0.0f;
    float destroy_value = 0.0f;
    window.keyboard.space.on_press.emplace_back([&]
    {
        destroy_goal = 1.5f;
    });

    camera.origin = { -2.0f, 2.0f, -2.0f };
    camera.set_forward(camera.origin * -1.0f);

    /* ----- UPDATE ----- */
    while (window.process(false)) {
        timer.update_delta();

        main_entity->update_physics(timer.delta());

        if (destroy_value < destroy_goal) {
            destroy_value += timer.delta() * 5.0f;
        }
        else {
            destroy_goal = -INFINITY;
            destroy_value -= timer.delta() * 0.25f;
        }

        sun_direction = kl::normalize(sun_direction);

        gpu.clear_internal(kl::colors::GRAY);

        struct VSData
        {
            kl::Float4x4 w_matrix;
            kl::Float4x4 vp_matrix;
            kl::Float4 misc_data;
        } vs_data = {};

        vs_data.vp_matrix = camera.matrix();
        vs_data.w_matrix = main_entity->matrix();
        vs_data.misc_data.x = max(destroy_value, 0.0f);
        default_shaders.vertex_shader.update_cbuffer(vs_data);

        struct PSData
        {
            kl::Float4 object_color;
            kl::Float4 sun_direction;
        } ps_data = {};

        ps_data.sun_direction = { sun_direction.x, sun_direction.y, sun_direction.z, 0.0f };
        ps_data.object_color = main_entity->material->color;
        default_shaders.pixel_shader.update_cbuffer(ps_data);

        if (main_entity->mesh) {
            gpu.draw(main_entity->mesh->graphics_buffer);
        }

        gpu.swap_buffers(true);
    }
    return 0;
}
