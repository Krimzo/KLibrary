#include "graphics/buffers/gpu_buffer.h"

#include "utility/utility.h"


// Makers
kl::ref<kl::gpu_buffer> kl::gpu_buffer::make(const dx::buffer_descriptor* descriptor, const dx::subresource_descriptor* subresource_data)
{
    return ref<gpu_buffer>(new gpu_buffer(descriptor, subresource_data));
}

kl::ref<kl::gpu_buffer> kl::gpu_buffer::make(const void* data, const UINT element_count, const UINT element_size, const bool has_unordered_access, const bool cpu_read)
{
    return ref<gpu_buffer>(new gpu_buffer(data, element_count, element_size, has_unordered_access, cpu_read));
}

kl::ref<kl::gpu_buffer> kl::gpu_buffer::make(const dx::buffer buffer, const UINT byte_size)
{
    return ref<gpu_buffer>(new gpu_buffer(buffer, byte_size));
}

// Class
kl::gpu_buffer::gpu_buffer(const dx::buffer_descriptor* descriptor, const dx::subresource_descriptor* subresource_data)
    : creation_type(gpu_buffer_creation_type::normal)
{
    const long result = BOUND_GPU->device()->CreateBuffer(descriptor, subresource_data, &child_object_);
    warning_check(!child_object_, format("Failed to create gpu buffer. Result: 0x", std::hex, result));
}

kl::gpu_buffer::gpu_buffer(const void* data, const UINT element_count, const UINT element_size, const bool has_unordered_access, const bool cpu_read)
    : creation_type(gpu_buffer_creation_type::structured)
{
    dx::buffer_descriptor descriptor = {};
    descriptor.Usage = D3D11_USAGE_DEFAULT;
    descriptor.BindFlags = D3D11_BIND_SHADER_RESOURCE | (has_unordered_access ? D3D11_BIND_UNORDERED_ACCESS : NULL);
    descriptor.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    descriptor.CPUAccessFlags = cpu_read ? D3D11_CPU_ACCESS_READ : NULL;
    descriptor.StructureByteStride = element_size;
    descriptor.ByteWidth = element_count * element_size;

    dx::subresource_descriptor subresource = {};
    subresource.pSysMem = data;

    this->gpu_buffer::gpu_buffer(&descriptor, &subresource);
}

kl::gpu_buffer::gpu_buffer(const dx::buffer buffer, const UINT byte_size)
    : creation_type(gpu_buffer_creation_type::staging)
{
    dx::buffer_descriptor descriptor = {};
    buffer->GetDesc(&descriptor);

    dx::buffer_descriptor staging_descriptor = {};
    staging_descriptor.Usage = D3D11_USAGE_STAGING;
    staging_descriptor.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    staging_descriptor.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    staging_descriptor.StructureByteStride = descriptor.StructureByteStride;
    staging_descriptor.ByteWidth = byte_size ? byte_size : descriptor.ByteWidth;

    this->gpu_buffer::gpu_buffer(&descriptor, nullptr);
}

UINT kl::gpu_buffer::get_size() const
{
    if (!*this) { return 0; }

    dx::buffer_descriptor descriptor = {};
    child_object_->GetDesc(&descriptor);
    return descriptor.ByteWidth;
}
