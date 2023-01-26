#include "klib.h"


struct example_struct
{
	kl::float3 position;
};

int main()
{
	constexpr int data_size = 10;
	example_struct example_data[data_size] = {};

	kl::gpu gpu = {};

	const kl::dx::buffer buffer = gpu.new_structured_buffer(example_data, data_size, sizeof(example_struct), true, true);
	const kl::dx::access_view shader_view = gpu.new_access_view(buffer);

	const kl::dx::compute_shader compute_shader = gpu.new_compute_shader(kl::files::read_string("examples/shaders/compute_test2.hlsl"));
	gpu.bind_compute_access_view(shader_view, 0);
	gpu.execute_compute_shader(compute_shader, { data_size, 1, 1 });

	gpu.read_from_resource(example_data, buffer, sizeof(example_data));

	for (auto& [position] : example_data) {
		print(position);
	}

	kl::get();
}
