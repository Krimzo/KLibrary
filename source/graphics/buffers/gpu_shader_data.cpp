#include "graphics/buffers/gpu_shader_data.h"

#include "utility/utility.h"


// Makers
kl::ref<kl::gpu_shader_data> kl::gpu_shader_data::make(const char shader_type, const std::string& source)
{
    return ref<gpu_shader_data>(new gpu_shader_data(shader_type, source));
}

// Class
kl::gpu_shader_data::gpu_shader_data(const char shader_type, const std::string& source)
{
    const std::string shader_name = format(shader_type, "_shader");
    const std::string shader_version = format(shader_type, "s_5_0");

    D3DCompile(source.c_str(), source.size(), nullptr, nullptr, nullptr, shader_name.c_str(), shader_version.c_str(), NULL, NULL, &data_, &error_);
    warning_check(!data_, "Failed to compile " + shader_name + ". Error: " + get_error());
}

kl::gpu_shader_data::~gpu_shader_data()
{
    safe_release(data_);
    safe_release(error_);
}

bool kl::gpu_shader_data::has_data() const
{
    return (bool) data_;
}

bool kl::gpu_shader_data::has_error() const
{
    return (bool) error_;
}

void* kl::gpu_shader_data::get_data() const
{
    return (data_ ? data_->GetBufferPointer() : nullptr);
}

std::string kl::gpu_shader_data::get_error() const
{
    return (error_ ? ((const char*) error_->GetBufferPointer()) : "Unknown");
}

SIZE_T kl::gpu_shader_data::data_size() const
{
    return (data_ ? data_->GetBufferSize() : 0);
}
