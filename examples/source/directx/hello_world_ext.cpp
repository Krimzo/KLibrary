#include "examples.h"


static constexpr auto SHADER_SOURCE =
R"(
struct VS_OUT
{
    float4 position : SV_Position;
    float3 color : VS_Color;
};

float4 HIGHLIGHT_COLOR;
float2 MOUSE_POSITION;

VS_OUT v_shader(float3 position : KL_Position, float3 normal : KL_Normal)
{
    VS_OUT data;
    data.position = float4(position, 1.0f);
    data.color = normal;
    return data;
}

float4 p_shader(VS_OUT data) : SV_Target0
{
    if (length(MOUSE_POSITION - data.position.xy) < 50.0f)
        return HIGHLIGHT_COLOR;
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
        struct alignas(16) CB
        {
            kl::Float4 HIGHLIGHT_COLOR;
            kl::Float2 MOUSE_POSITION;
        } cb = {};

        cb.HIGHLIGHT_COLOR = kl::colors::GRAY;
        cb.MOUSE_POSITION = window.mouse.position();
        shaders.upload(cb);
        
        gpu.clear_internal(kl::colors::GRAY);
        gpu.draw_indexed(vertex_buffer, index_buffer);
        gpu.swap_buffers(true);
    }
    return 0;
}
