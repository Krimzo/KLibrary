#include "examples.h"


static const char* SHADER_SOURCE =
R"(
struct VS_OUT
{
	float4 position : SV_Position;
	float3 color : VS_Color;
};

// Vertex shader
VS_OUT v_shader(const float3 position : KL_Position, const float3 color : KL_Normal)
{
	VS_OUT data;
	data.position = float4(position, 1.0f);
	data.color = color;
    return data;
}

// Pixel shader
float4 p_shader(const VS_OUT data) : SV_Target
{
    return float4(data.color, 1.0f);
}
)";

/* NOTE: KL_DX12 is in an early dev stage, don't expect many features */

int examples::hello_world_12_main()
{
	kl::Window window{ "Hello World! (D3D12)", { 1600, 900 } };
	window.set_resizeable(false);

	kl::GPU12 gpu{ static_cast<HWND>(window), kl::IS_DEBUG };
	auto& queue = gpu.queue;
	auto& commands = gpu.commands;
	auto& fence = gpu.fence;

	// Vertex data
	const kl::Vertex vertices[3] = {
		kl::Vertex{ kl::Float3{ -0.5f, -0.5f, 0.0f }, {}, kl::Float3{ kl::colors::RED } },
		kl::Vertex{ kl::Float3{  0.0f,  0.5f, 0.0f }, {}, kl::Float3{ kl::colors::GREEN } },
		kl::Vertex{ kl::Float3{  0.5f, -0.5f, 0.0f }, {}, kl::Float3{ kl::colors::BLUE } },
	};
	const std::pair vertex_buffer = gpu.create_vertex_buffer(vertices, (UINT) std::size(vertices));

	// Pipeline
	const kl::dx12::RootSignature root_signature = gpu.create_root_signature();
	const kl::dx12::PipelineState pipeline_state = gpu.create_default_rasterization_pipeline(root_signature, SHADER_SOURCE);

	// Render loop
	while (window.process(false)) {
		const UINT back_buffer_index = gpu.back_buffer_index();
		const kl::dx12::Resource back_buffer = gpu.get_back_buffer(back_buffer_index);
		const kl::dx12::DescriptorHandle render_target = gpu.get_render_target(back_buffer_index);

		commands.reset();
		commands.transition_resource(back_buffer, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

		commands.set_render_target(&render_target, nullptr);
		commands.clear_target_view(render_target, kl::Color(30, 30, 30));

		commands.set_scissors(kl::dx12::Scissors{ 0, 0, LONG_MAX, LONG_MAX });
		commands.set_viewport(kl::dx12::Viewport{ 0.0f, 0.0f, (float) window.width(), (float) window.height() });

		commands.set_root_signature(root_signature);
		commands.set_pipeline_state(pipeline_state);

		commands.set_vertex_buffer(vertex_buffer.second);
		commands.set_primitive_topology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		commands.draw((UINT) std::size(vertices));

		commands.transition_resource(back_buffer, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		commands.close();

		queue.execute(commands.list);
		fence.signal_and_wait(queue.queue);
		gpu.swap_buffers(true);
	}

	fence.signal_and_wait(queue.queue);
	return 0;
}
