#include "klib.h"


static const std::string shader_source = R"(
// Vertex shader
float4 v_shader(const float3 position : KL_Position) : SV_Position
{
    return float4(position, 1);
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
    float4 object_color;
    float4 highlight_color;
};

float4 p_shader(const float4 screen_position : SV_Position) : SV_Target
{
    if (length(mouse_position.xy - screen_position.xy) < 50) {
        return highlight_color;
    }
    return object_color;
}
)";

struct ps_cb
{
    kl::float4 mouse_position;
    kl::float4 object_color;
    kl::float4 highlight_color;
};

int main()
{
    // Window setup
    kl::window window = { { 1600, 900 }, "Hello World" };
    kl::gpu gpu = { (HWND) window };

    // Window resize setup
    window.on_resize.push_back([&](kl::int2 new_size)
    {
        if (new_size.x > 0 && new_size.y > 0) {
            gpu.resize_internal(new_size);
            gpu.set_viewport_size(new_size);
        }
    });
    window.maximize();

    // Fullscreen setup
    window.keyboard.f11.on_press.push_back([&]
    {
        const bool new_state = !window.in_fullscreen();
        window.set_fullscreen(new_state);
        gpu.set_fullscreen(new_state);
    });

    // Mesh setup
    auto triangle = gpu.create_vertex_buffer({
        { {  0.0f,  0.5f, 0.5f } },
        { { -0.5f, -0.5f, 0.5f } },
        { {  0.5f, -0.5f, 0.5f } },
    });

    // Shader setup
    auto shaders = gpu.create_render_shaders(shader_source);
    gpu.bind_render_shaders(shaders);

    // CDS (Clear-Draw-Swap)
    while (window.process(false)) {
        ps_cb ps_data = {};
        ps_data.mouse_position = { window.mouse.position(), 0, 0 };
        ps_data.object_color = { 1, 0.75f, 0.35f, 1 };
        ps_data.highlight_color = { 0.35f, 0.75f, 1, 1 };
        shaders.pixel_shader.update_cbuffer(ps_data);
        
        gpu.clear_internal(kl::colors::gray);
        gpu.draw_mesh(triangle);
        gpu.swap_buffers(true);
    }
}
