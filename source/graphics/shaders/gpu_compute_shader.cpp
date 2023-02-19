#include "graphics/shaders/gpu_compute_shader.h"

#include "graphics/buffers/gpu_shader_data.h"
#include "utility/utility.h"


// Makers
kl::ref<kl::gpu_compute_shader> kl::gpu_compute_shader::make(const std::string& source)
{
    return ref<gpu_compute_shader>(new gpu_compute_shader(source));
}

// Class
kl::gpu_compute_shader::gpu_compute_shader(const std::string& source)
{
    const auto shader_data = gpu_shader_data::make('c', source);
    BOUND_GPU->device()->CreateComputeShader(shader_data->get_data(), shader_data->data_size(), nullptr, &child_object_);
    warning_check(!child_object_, "Failed to create compute shader");
}

void kl::gpu_compute_shader::bind() const
{
    BOUND_GPU->context()->CSSetShader(child_object_, nullptr, 0);
}

void kl::gpu_compute_shader::unbind()
{
    BOUND_GPU->context()->CSSetShader(nullptr, nullptr, 0);
}

void kl::gpu_compute_shader::dispatch(const UINT x, const UINT y, const UINT z) const
{
    BOUND_GPU->context()->Dispatch(x, y, z);
}

void kl::gpu_compute_shader::execute(const UINT x, const UINT y, const UINT z) const
{
    bind();
    dispatch(x, y, z);
}
