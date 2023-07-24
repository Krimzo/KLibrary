#include "examples.h"


static const std::string shader_source =
R"(
struct VS_OUT
{
    float4 position : SV_Position;
    float3 color : VS_Color;
};

// Vertex shader
VS_OUT v_shader(const float3 position : KL_Position, const float3 normal : KL_Normal)
{
    VS_OUT data;
    data.position = float4(position, 1.0f);
    data.color = normal;
    return data;
}

// Pixel shader
/*
NOTE:
Direct3D cbuffers use special type of data packing.
In short, all cbuffers will be packed into a byte size that is a multiple of 16.
It is preferred to use float4 or float4x4 since each already have a size that is a multiple of 16.
Refer to: https://learn.microsoft.com/en-us/windows/win32/direct3dhlsl/dx-graphics-hlsl-packing-rules
*/

cbuffer ps_cb : register(b0)
{
    float4 mouse_position;
    float4 highlight_color;
};

float4 p_shader(const VS_OUT data) : SV_Target
{
    const float4 object_color = float4(data.color, 1.0f);
    if (length(mouse_position.xy - data.position.xy) < 50) {
        return highlight_color;
    }
    return object_color;
}
)";

struct ps_cb
{
    kl::float4 mouse_position;
    kl::float4 highlight_color;
};

int examples::hello_world_ext_main()
{
    // Window setup
    kl::window window = { { 1600, 900 }, "Hello World! (Extended)" };
    kl::gpu gpu = { (HWND) window };

    // Window resize setup
    window.on_resize.emplace_back([&](kl::int2 new_size)
    {
        if (new_size.x > 0 && new_size.y > 0) {
            gpu.resize_internal(new_size);
            gpu.set_viewport_size(new_size);
        }
    });
    window.maximize();

    // Fullscreen setup
    window.keyboard.f11.on_press.emplace_back([&]
    {
        const bool new_state = !window.in_fullscreen();
        window.set_fullscreen(new_state);
        gpu.set_fullscreen(new_state);
    });

    // Mesh setup
    const std::vector<kl::vertex> vertices = {
        { { -0.5f, -0.5f, 0.5f }, {}, kl::colors::red },
        { { -0.5f,  0.5f, 0.5f }, {}, kl::colors::green },
        { {  0.5f, -0.5f, 0.5f }, {}, kl::colors::blue },
        { {  0.5f,  0.5f, 0.5f }, {}, kl::colors::white },
    };
    const std::vector<uint32_t> indices = {
        0, 1, 3,
        0, 2, 3,
    };

    kl::dx::buffer vertex_buffer = gpu.create_vertex_buffer(vertices);
    kl::dx::buffer index_buffer = gpu.create_index_buffer(indices);

    // Shader setup
    kl::render_shaders shaders = gpu.create_render_shaders(shader_source);
    gpu.bind_render_shaders(shaders);

    // CDS (Clear-Draw-Swap)
    while (window.process(false)) {
        ps_cb ps_data = {};
        ps_data.mouse_position = { window.mouse.position(), 0, 0 };
        ps_data.highlight_color = (kl::float4) kl::colors::gray;
        shaders.pixel_shader.update_cbuffer(ps_data);
        
        gpu.clear_internal(kl::colors::gray);
        gpu.draw_indexed(vertex_buffer, index_buffer);
        gpu.swap_buffers(true);
    }
    return 0;
}
