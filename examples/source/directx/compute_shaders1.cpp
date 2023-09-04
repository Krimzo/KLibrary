#include "examples.h"


struct example_struct
{
    kl::float3 position;
};

int examples::compute_shaders1_main()
{
    kl::gpu gpu = {};

    // CPU buffer
    static constexpr int data_count = 10;
    example_struct example_data[data_count] = {};

    // GPU buffer
    kl::dx::buffer buffer = gpu.create_structured_buffer(example_data, data_count, sizeof(example_struct), true, true);
    kl::dx::access_view access_view = gpu.create_access_view(buffer, nullptr);
    gpu.bind_access_view_for_compute_shader(access_view, 0);

    // Compiled shader
    const std::string shader_source = kl::read_file_string("shaders/compute_test1.hlsl");
    kl::shader_holder compute_shader = gpu.create_compute_shader(shader_source);

    // Run shader
    gpu.bind_compute_shader(compute_shader);
    gpu.dispatch_compute_shader(data_count, 1, 1);

    // Copy from GPU to CPU
    gpu.read_from_resource(example_data, buffer, (data_count * sizeof(example_struct)));

    // Print values
    for (auto& [position] : example_data) {
        print(position);
    }
    kl::get();
    return 0;
}
