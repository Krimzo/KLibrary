#include "graphics/shaders/gpu_pixel_shader.h"

#include "graphics/buffers/gpu_shader_data.h"
#include "utility/utility.h"


// Makers
kl::ref<kl::gpu_pixel_shader> kl::gpu_pixel_shader::make(const std::string& source)
{
    return ref<gpu_pixel_shader>(new gpu_pixel_shader(source));
}

// Class
kl::gpu_pixel_shader::gpu_pixel_shader(const std::string& source)
{
    const auto shader_data = gpu_shader_data::make('p', source);
    BOUND_GPU->device()->CreatePixelShader(shader_data->get_data(), shader_data->data_size(), nullptr, &child_object_);
    warning_check(!child_object_, "Failed to create pixel shader");
}

void kl::gpu_pixel_shader::bind() const
{
    BOUND_GPU->context()->PSSetShader(child_object_, nullptr, 0);
}

void kl::gpu_pixel_shader::unbind() const
{
    BOUND_GPU->context()->PSSetShader(nullptr, nullptr, 0);
}
