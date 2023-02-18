#include "klib.h"


struct vs_cb
{
    kl::float4x4 w_matrix;
    kl::float4x4 vp_matrix;
    kl::float4 misc_data;
};
static kl::ref<kl::gpu_const_buffer> vs_cbuffer = nullptr;

struct ps_cb
{
    kl::float4 object_color;
    kl::float4 sun_direction;
};
static kl::ref<kl::gpu_const_buffer> ps_cbuffer = nullptr;

static kl::timer timer = {};
static kl::camera camera = {};

static kl::float3 sun_direction = { 1.0f, -1.0f, 0.0f };

/* IMPORTANT
*
CURRENTLY ONLY WORKING EXAMPLE..
..will fix them all tomorrow.

--------- */

int main()
{
    /* ----- SETUP ----- */
    kl::BOUND_WINDOW = kl::window::make({ 1600, 900 }, "Geometry Test");
    kl::BOUND_GPU = kl::gpu::make(*kl::BOUND_WINDOW);

    auto& window = kl::BOUND_WINDOW;
    auto& gpu = kl::BOUND_GPU;

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
    window->keyboard.v.on_press[0]();

    std::string shaders_source = kl::files::read_string("examples/shaders/geometry_test.hlsl");
    auto default_shaders = kl::gpu_shaders::make(shaders_source);
    auto geometry_shader = kl::gpu_geometry_shader::make(shaders_source);

    default_shaders->bind();
    geometry_shader->bind();

    vs_cbuffer = kl::gpu_const_buffer::make(sizeof(vs_cb));
    ps_cbuffer = kl::gpu_const_buffer::make(sizeof(ps_cb));

    vs_cbuffer->bind_for_vertex_shader(0);
    ps_cbuffer->bind_for_pixel_shader(0);

    auto cube_mesh = kl::gpu_mesh::make("examples/meshes/cube.obj", true);
    auto sphere_mesh = kl::gpu_mesh::make("examples/meshes/sphere.obj", true);
    auto monke_mesh = kl::gpu_mesh::make("examples/meshes/monke.obj", true);

    kl::ref<kl::entity> main_entity = kl::make<kl::entity>();
    main_entity->angular.y = -36.0f;
    main_entity->mesh = monke_mesh;
    main_entity->material->color = kl::float4(kl::colors::orange);

    window->keyboard.num1.on_press.push_back([&]
    {
        main_entity->mesh = cube_mesh;
    });
    window->keyboard.num2.on_press.push_back([&]
    {
        main_entity->mesh = sphere_mesh;
    });
    window->keyboard.num3.on_press.push_back([&]
    {
        main_entity->mesh = monke_mesh;
    });

    float destroy_goal = 0.0f;
    float destroy_value = 0.0f;
    window->keyboard.space.on_press.push_back([&]
    {
        destroy_goal = 1.5f;
    });

    camera.position = { -2.0f, 2.0f, -2.0f };
    camera.set_forward(camera.position * -1.0f);

    /* ----- UPDATE ----- */
    while (window->process(false)) {
        timer.update_interval();

        main_entity->update_physics(timer.get_interval());

        if (destroy_value < destroy_goal) {
            destroy_value += timer.get_interval() * 5.0f;
        }
        else {
            destroy_goal = -INFINITY;
            destroy_value -= timer.get_interval() * 0.25f;
        }

        sun_direction = kl::math::normalize(sun_direction);

        gpu->clear_internal_color((kl::float4) kl::colors::gray);
        gpu->clear_internal_depth(1.0f, 0xFF);

        vs_cb vs_data = {};
        vs_data.vp_matrix = camera.matrix();

        ps_cb ps_data = {};
        ps_data.sun_direction = { sun_direction.x, sun_direction.y, sun_direction.z, 0.0f };

        vs_data.w_matrix = main_entity->matrix();
        vs_data.misc_data.x = max(destroy_value, 0.0f);
        vs_cbuffer->set_data(vs_data);

        ps_data.object_color = main_entity->material->color;
        ps_cbuffer->set_data(ps_data);

        if (main_entity->mesh) {
            main_entity->mesh->draw();
        }

        gpu->swap_buffers(true);
    }
}
