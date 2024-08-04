#include "examples.h"


int examples::compute_shaders1_main(const int argc, const char** argv)
{
    struct TestStruct
    {
        kl::Float3 position;
    };

    kl::GPU gpu;

    // CPU buffer
    static constexpr int data_count = 10;
    TestStruct example_data[data_count] = {};

    // GPU buffer
    const kl::dx::Buffer buffer = gpu.create_structured_buffer(example_data, data_count, sizeof(TestStruct), true, true);
    const kl::dx::AccessView access_view = gpu.create_access_view(buffer, nullptr);
    gpu.bind_access_view_for_compute_shader(access_view, 0);

    // Compiled shader
    const std::string shader_source = kl::read_file_string("shaders/compute_test1.hlsl");
    const kl::ShaderHolder compute_shader = gpu.create_compute_shader(shader_source);

    // Run shader
    gpu.bind_compute_shader(compute_shader);
    gpu.dispatch_compute_shader(data_count, 1, 1);

    // Copy from GPU to CPU
    gpu.read_from_buffer(example_data, buffer, data_count * sizeof(TestStruct));

    // Print values
    for (auto& [position] : example_data) {
        print(position);
    }
    return 0;
}
