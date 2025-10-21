#include "examples.h"


static constexpr auto SHADER_SOURCE =
R"(
struct VS_OUT
{
	float4 position : SV_Position;
	float3 color : VS_Color;
};

VS_OUT v_shader(float3 position : KL_Position, float3 color : KL_Normal)
{
	VS_OUT data;
	data.position = float4(position, 1.0f);
	data.color = color;
    return data;
}

float4 p_shader(VS_OUT data) : SV_Target
{
    return float4(data.color, 1.0f);
}
)";

int examples::hello_world_12_main( int argc, char** argv )
{
    kl::Window window{ "Hello World! (D3D12)" };
    kl::GPU12 gpu{ window.ptr() };

    window.on_resize.emplace_back( [&]( kl::Int2 size )
        {
            gpu.resize( size );
        } );

    kl::Vertex vertices[3] = {
        kl::Vertex{ kl::Float3{ -0.5f, -0.5f, 0.0f }, kl::Float3{ kl::colors::RED }, {} },
        kl::Vertex{ kl::Float3{ 0.0f, 0.5f, 0.0f }, kl::Float3{ kl::colors::GREEN }, {} },
        kl::Vertex{ kl::Float3{ 0.5f, -0.5f, 0.0f }, kl::Float3{ kl::colors::BLUE }, {} },
    };
    std::pair vertex_buffer = gpu.create_vertex_buffer( vertices, (UINT) std::size( vertices ) );

    kl::dx12::RootSignature root_signature = gpu.create_root_signature();
    kl::dx12::PipelineState pipeline_state = gpu.create_default_rasterization_pipeline( root_signature, SHADER_SOURCE );

    while ( window.process() )
    {
        UINT back_buffer_index = gpu.back_buffer_index();
        kl::dx12::Resource back_buffer = gpu.get_back_buffer( back_buffer_index );
        kl::dx12::DescriptorHandle render_target = gpu.get_render_target( back_buffer_index );

        gpu.execute( [&]( auto& commands )
            {
                commands.transition_resource( back_buffer, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET );
                commands.set_render_target( &render_target, nullptr );
                commands.clear_target_view( render_target, kl::RGB( 30, 30, 30 ) );

                commands.set_scissors( kl::dx12::Scissors{ 0, 0, LONG_MAX, LONG_MAX } );
                commands.set_viewport( kl::dx12::Viewport{ 0.0f, 0.0f, (float) window.width(), (float) window.height(), 0.0f, 1.0f } );

                commands.set_root_signature( root_signature );
                commands.set_pipeline_state( pipeline_state );

                commands.set_vertex_buffer( vertex_buffer.second );
                commands.set_primitive_topology( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
                commands.draw( (UINT) std::size( vertices ) );
                commands.transition_resource( back_buffer, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT );
            } );

        gpu.swap_buffers( true );
    }
    return 0;
}
