#include "examples.h"


static constexpr auto SHADER_SOURCE =
R"(
struct VS_OUT
{
    float4 position : SV_Position;
    float3 color : VS_Color;
};

VS_OUT v_shader(const float3 position : KL_Position, const float3 normal : KL_Normal)
{
    VS_OUT data;
    data.position = float4(position, 1.0f);
    data.color = normal;
    return data;
}

/*
NOTE:
Direct3D cbuffers use special type of data packing.
In short, all cbuffers will be packed into a byte size that is a multiple of 16.
It is preferred to use float4 or float4x4 since each already have a size that is a multiple of 16.
Refer to: https://learn.microsoft.com/en-us/windows/win32/direct3dhlsl/dx-graphics-hlsl-packing-rules
*/

cbuffer PS_CB : register(b0)
{
    float2 MOUSE_POSITION;
    float3 HIGHLIGHT_COLOR;
};

float4 p_shader(const VS_OUT data) : SV_Target
{
    if (length(MOUSE_POSITION - data.position.xy) < 50.0f) {
        return float4(HIGHLIGHT_COLOR, 1.0f);
    }
    return float4(data.color, 1.0f);
}
)";

int examples::hello_world_ext_main(const int argc, const char** argv)
{
    kl::Window window = { "Hello World! (D3D11 Extended)", { 1600, 900 } };
    kl::GPU gpu = { HWND(window), kl::IS_DEBUG };

    window.on_resize.emplace_back([&](const kl::Int2 new_size)
    {
        if (new_size.x > 0 && new_size.y > 0) {
            gpu.resize_internal(new_size);
            gpu.set_viewport_size(new_size);
        }
    });

    const std::vector<kl::Vertex<float>> vertices = {
        { { -0.5f, -0.5f, 0.5f }, {}, kl::colors::RED },
        { { -0.5f,  0.5f, 0.5f }, {}, kl::colors::GREEN },
        { {  0.5f, -0.5f, 0.5f }, {}, kl::colors::BLUE },
        { {  0.5f,  0.5f, 0.5f }, {}, kl::colors::WHITE },
    };
    const std::vector<uint32_t> indices = {
        0, 1, 3,
        0, 2, 3,
    };

    const kl::dx::Buffer vertex_buffer = gpu.create_vertex_buffer(vertices);
    const kl::dx::Buffer index_buffer = gpu.create_index_buffer(indices);

    kl::RenderShaders shaders = gpu.create_render_shaders(SHADER_SOURCE);
    gpu.bind_render_shaders(shaders);

    while (window.process()) {
        struct PS_CB
        {
            kl::Float2 MOUSE_POSITION;
            alignas(16) kl::Float3 HIGHLIGHT_COLOR;
        };
		const PS_CB ps_cb{
            .MOUSE_POSITION = window.mouse.position(),
			.HIGHLIGHT_COLOR = (kl::Float3) kl::colors::GRAY,
        };
        shaders.pixel_shader.update_cbuffer(ps_cb);
        
        gpu.clear_internal(kl::colors::GRAY);
        gpu.draw_indexed(vertex_buffer, index_buffer);
        gpu.swap_buffers(true);
    }
    return 0;
}
