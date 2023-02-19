#include "graphics/buffers/gpu_const_buffer.h"

#include "utility/utility.h"


// Makers
kl::ref<kl::gpu_const_buffer> kl::gpu_const_buffer::make(UINT byte_size)
{
    return ref<gpu_const_buffer>(new gpu_const_buffer(byte_size));
}

// Class
kl::gpu_const_buffer::gpu_const_buffer(const UINT byte_size)
{
    if (warning_check(byte_size % 16, "Constant buffer size has to be a multiple of 16")) { return; }

    dx::buffer_descriptor descriptor = {};
    descriptor.ByteWidth = byte_size;
    descriptor.Usage = D3D11_USAGE_DYNAMIC;
    descriptor.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    descriptor.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    buffer_ = gpu_buffer::make(&descriptor, nullptr);
}

kl::gpu_const_buffer::operator bool() const
{
    return (buffer_ ? ((bool) *buffer_) : false);
}

UINT kl::gpu_const_buffer::get_size() const
{
    return buffer_ ? buffer_->get_size() : 0;
}

void kl::gpu_const_buffer::bind_for_vertex_shader(UINT slot) const
{
    BOUND_GPU->context()->VSSetConstantBuffers(slot, 1, *buffer_);
}

void kl::gpu_const_buffer::bind_for_geometry_shader(UINT slot) const
{
    BOUND_GPU->context()->GSSetConstantBuffers(slot, 1, *buffer_);
}

void kl::gpu_const_buffer::bind_for_pixel_shader(UINT slot) const
{
    BOUND_GPU->context()->PSSetConstantBuffers(slot, 1, *buffer_);
}

void kl::gpu_const_buffer::bind_for_compute_shader(UINT slot) const
{
    BOUND_GPU->context()->CSSetConstantBuffers(slot, 1, *buffer_);
}

void kl::gpu_const_buffer::unbind_for_vertex_shader(const UINT slot)
{
    const dx::buffer buffer = nullptr;
    BOUND_GPU->context()->VSSetConstantBuffers(slot, 1, &buffer);
}

void kl::gpu_const_buffer::unbind_for_geometry_shader(const UINT slot)
{
    const dx::buffer buffer = nullptr;
    BOUND_GPU->context()->GSSetConstantBuffers(slot, 1, &buffer);
}

void kl::gpu_const_buffer::unbind_for_pixel_shader(const UINT slot)
{
    const dx::buffer buffer = nullptr;
    BOUND_GPU->context()->PSSetConstantBuffers(slot, 1, &buffer);
}

void kl::gpu_const_buffer::unbind_for_compute_shader(const UINT slot)
{
    const dx::buffer buffer = nullptr;
    BOUND_GPU->context()->CSSetConstantBuffers(slot, 1, &buffer);
}

void kl::gpu_const_buffer::set_data(const void* data, UINT byte_size) const
{
    if (!buffer_) { return; }

    const UINT buffer_size = get_size();
    if (byte_size == 0 || byte_size > buffer_size) {
        byte_size = buffer_size;
    }
    BOUND_GPU->write_to_resource(*buffer_, data, byte_size);
}
