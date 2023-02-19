#include "klib.h"


struct example_struct
{
    kl::float3 position;
};

int main()
{
    static constexpr int data_size = 10;
    example_struct example_data[data_size] = {};

    kl::BOUND_GPU = kl::gpu::make();
    auto& gpu = *kl::BOUND_GPU;

    auto buffer = kl::gpu_buffer::make(example_data, data_size, sizeof(example_struct), true, true);
    auto access_view = kl::gpu_access_view::make(*buffer, nullptr);
    access_view->bind(0);

    auto compute_shader = kl::gpu_compute_shader::make(kl::files::read_string("examples/shaders/compute_test2.hlsl"));
    compute_shader->execute(data_size, 1, 1);

    gpu.read_from_resource(example_data, *buffer, sizeof(example_data));

    for (auto& [position] : example_data) {
        print(position);
    }


    kl::get();
}
