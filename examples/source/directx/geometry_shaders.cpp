#include "examples.h"


int examples::geometry_shaders_main(const int argc, const char** argv)
{
    kl::Window window{ "Geometry Test" };
    kl::GPU gpu{ window.ptr() };
    kl::Timer timer;
    kl::Camera camera;

    window.on_resize.emplace_back([&](kl::Int2 size)
    {
        gpu.resize_internal(size);
        gpu.set_viewport_size(size);
        camera.update_aspect_ratio(size);
    });
    window.maximize();

    std::string shader_sources = kl::read_file("shaders/geometry_test.hlsl");
    kl::Shaders default_shaders = gpu.create_shaders(shader_sources);
    kl::GeometryShader geometry_shader = gpu.create_geometry_shader(shader_sources);
    gpu.bind_shaders(default_shaders);
    gpu.bind_geometry_shader(geometry_shader.shader);

    kl::Ref cube_mesh = new kl::Mesh(gpu);
    kl::Ref sphere_mesh = new kl::Mesh(gpu);
    kl::Ref monke_mesh = new kl::Mesh(gpu);
    cube_mesh->buffer = gpu.create_vertex_buffer("meshes/cube.obj");
    sphere_mesh->buffer =gpu.create_vertex_buffer("meshes/sphere.obj");
    monke_mesh->buffer =gpu.create_vertex_buffer("meshes/monke.obj");

    kl::Ref default_material = new kl::Material();
    default_material->color = kl::colors::ORANGE;

    kl::Ref main_entity = new kl::Entity();
    main_entity->angular.y = -36.0f;
    main_entity->mesh = monke_mesh;
    main_entity->material = default_material;

    float destroy_goal = 0.0f;
    float destroy_value = 0.0f;
    camera.position = { -2.0f, 2.0f, -2.0f };
    camera.set_forward(camera.position * -1.0f);

    const kl::Float3 sun_direction = kl::normalize(kl::Float3{ 1.0f, -1.0f, 0.0f });

    while (window.process()) {
        timer.update();

        if (window.keyboard.v.pressed()) {
            static bool wireframe_bound = true;
            static kl::dx::RasterState solid_raster = gpu.create_raster_state(false, false);
            static kl::dx::RasterState wireframe_raster = gpu.create_raster_state(true, false);
            gpu.bind_raster_state(wireframe_bound ? solid_raster : wireframe_raster);
            wireframe_bound = !wireframe_bound;
        }
        if (window.keyboard.one.pressed()) {
            main_entity->mesh = cube_mesh;
        }
        if(window.keyboard.two.pressed()) {
			main_entity->mesh = sphere_mesh;
        }
        if (window.keyboard.three.pressed()) {
			main_entity->mesh = monke_mesh;
        }
        if (window.keyboard.space.pressed()) {
            destroy_goal = 1.5f;
        }

        main_entity->update_physics(timer.delta());

        if (destroy_value < destroy_goal) {
            destroy_value += timer.delta() * 5.0f;
        }
        else {
            destroy_goal = -std::numeric_limits<float>::infinity();
            destroy_value -= timer.delta() * 0.25f;
        }

        gpu.clear_internal(kl::colors::GRAY);

        if (main_entity->mesh) {
            struct alignas(16) CB
            {
                kl::Float4x4 W;
                kl::Float4x4 VP;
                kl::Float4 MISC_DATA;
                kl::Float4 OBJECT_COLOR;
                kl::Float4 SUN_DIRECTION;
            } cb = {};

            cb.W = main_entity->matrix();
            cb.VP = camera.matrix();
            cb.MISC_DATA.x = kl::max(destroy_value, 0.0f);
            cb.OBJECT_COLOR = main_entity->material->color;
            cb.SUN_DIRECTION = { sun_direction.x, sun_direction.y, sun_direction.z, 0.0f };

            default_shaders.upload(cb);
            gpu.draw(main_entity->mesh->buffer);
        }

        gpu.swap_buffers(true);
    }
    return 0;
}
