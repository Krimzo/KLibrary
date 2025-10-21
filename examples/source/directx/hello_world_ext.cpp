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

int examples::hello_world_ext_main( int argc, char** argv )
{
    kl::Window window{ "Hello World! (D3D11 Extended)" };
    kl::GPU gpu{ window.ptr() };

    window.on_resize.emplace_back( [&]( kl::Int2 size )
        {
            gpu.resize_internal( size );
            gpu.set_viewport_size( size );
        } );

    std::vector<kl::Vertex> vertices = {
        { { -0.5f, -0.5f, 0.5f }, kl::colors::RED, {} },
        { { -0.5f, 0.5f, 0.5f }, kl::colors::GREEN, {} },
        { { 0.5f, -0.5f, 0.5f }, kl::colors::BLUE, {} },
        { { 0.5f, 0.5f, 0.5f }, kl::colors::WHITE, {} },
    };
    std::vector<uint32_t> indices = {
        0, 1, 3,
        0, 2, 3,
    };

    kl::dx::Buffer vertex_buffer = gpu.create_vertex_buffer( vertices );
    kl::dx::Buffer index_buffer = gpu.create_index_buffer( indices );

    kl::Shaders shaders = gpu.create_shaders( SHADER_SOURCE );
    gpu.bind_shaders( shaders );

    while ( window.process() )
    {
        struct alignas( 16 ) CB
        {
            kl::Float4 HIGHLIGHT_COLOR;
            kl::Float2 MOUSE_POSITION;
        } cb = {};

        cb.HIGHLIGHT_COLOR = kl::colors::GRAY;
        cb.MOUSE_POSITION = window.mouse.position();
        shaders.upload( cb );

        gpu.clear_internal( kl::colors::GRAY );
        gpu.draw_indexed( vertex_buffer, index_buffer );
        gpu.swap_buffers( true );
    }
    return 0;
}
