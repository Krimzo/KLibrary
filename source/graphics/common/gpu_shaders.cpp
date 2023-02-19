#include "graphics/common/gpu_shaders.h"

#include "graphics/buffers/gpu_shader_data.h"
#include "utility/utility.h"


// Makers
kl::ref<kl::gpu_shaders> kl::gpu_shaders::make(const std::string& source)
{
    return ref<gpu_shaders>(new gpu_shaders(source));
}

// Class
kl::gpu_shaders::gpu_shaders(const std::string& source)
{
    vertex_shader_ = gpu_vertex_shader::make(source);
    pixel_shader_ = gpu_pixel_shader::make(source);
}

kl::gpu_shaders::operator bool() const
{
    return (bool) vertex_shader_ && (bool) pixel_shader_;
}

void kl::gpu_shaders::bind() const
{
    if (vertex_shader_) {
        vertex_shader_->bind();
    }
    if (pixel_shader_) {
        pixel_shader_->bind();
    }
}

void kl::gpu_shaders::unbind()
{
    gpu_vertex_shader::unbind();
    gpu_pixel_shader::unbind();
}
