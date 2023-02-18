#include "graphics/shaders/gpu_geometry_shader.h"

#include "graphics/buffers/gpu_shader_data.h"
#include "utility/utility.h"


// Makers
kl::ref<kl::gpu_geometry_shader> kl::gpu_geometry_shader::make(const std::string& source)
{
    return ref<gpu_geometry_shader>(new gpu_geometry_shader(source));
}

// Class
kl::gpu_geometry_shader::gpu_geometry_shader(const std::string& source)
{
    const auto shader_data = gpu_shader_data::make('g', source);
    BOUND_GPU->device()->CreateGeometryShader(shader_data->get_data(), shader_data->data_size(), nullptr, &child_object_);
    warning_check(!child_object_, "Failed to create geometry shader");
}

void kl::gpu_geometry_shader::bind() const
{
    BOUND_GPU->context()->GSSetShader(child_object_, nullptr, 0);
}

void kl::gpu_geometry_shader::unbind() const
{
    BOUND_GPU->context()->GSSetShader(nullptr, nullptr, 0);
}
