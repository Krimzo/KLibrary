#include "graphics/views/gpu_shader_view.h"

#include "utility/utility.h"


// Makers
kl::ref<kl::gpu_shader_view> kl::gpu_shader_view::make(const dx::resource resource, const dx::shader_view_descriptor* descriptor)
{
    return ref<gpu_shader_view>(new gpu_shader_view(resource, descriptor));
}

// Class
kl::gpu_shader_view::gpu_shader_view(const dx::resource resource, const dx::shader_view_descriptor* descriptor)
{
    const long result = BOUND_GPU->device()->CreateShaderResourceView(resource, descriptor, &child_object_);
    warning_check(!child_object_, format("Failed to create shader view. Result: 0x", std::hex, result));
}

void kl::gpu_shader_view::bind_for_pixel_shader(const UINT slot) const
{
    BOUND_GPU->context()->PSSetShaderResources(slot, 1, &child_object_);
}

void kl::gpu_shader_view::bind_for_compute_shader(const UINT slot) const
{
    BOUND_GPU->context()->CSSetShaderResources(slot, 1, &child_object_);
}

void kl::gpu_shader_view::unbind_for_pixel_shader() const
{
    BOUND_GPU->context()->PSSetShaderResources(0, 0, nullptr);
}

void kl::gpu_shader_view::unbind_for_compute_shader() const
{
    BOUND_GPU->context()->CSSetShaderResources(0, 0, nullptr);
}
