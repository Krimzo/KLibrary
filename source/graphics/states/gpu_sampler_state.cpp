#include "graphics/states/gpu_sampler_state.h"

#include "utility/utility.h"


// Makers
kl::ref<kl::gpu_sampler_state> kl::gpu_sampler_state::make(const dx::sampler_state_descriptor* descriptor)
{
    return ref<gpu_sampler_state>(new gpu_sampler_state(descriptor));
}

kl::ref<kl::gpu_sampler_state> kl::gpu_sampler_state::make(bool linear, bool mirror)
{
    return ref<gpu_sampler_state>(new gpu_sampler_state(linear, mirror));
}

// Class
kl::gpu_sampler_state::gpu_sampler_state(const dx::sampler_state_descriptor* descriptor)
{
    const long result = BOUND_GPU->device()->CreateSamplerState(descriptor, &child_object_);
    warning_check(!child_object_, format("Failed to create sampler state. Result: 0x", std::hex, result));
}

kl::gpu_sampler_state::gpu_sampler_state(const bool linear, const bool mirror)
{
    dx::sampler_state_descriptor descriptor = {};

    descriptor.Filter = linear ? D3D11_FILTER_MIN_MAG_MIP_LINEAR : D3D11_FILTER_MIN_MAG_MIP_POINT;
    descriptor.AddressU = mirror ? D3D11_TEXTURE_ADDRESS_MIRROR : D3D11_TEXTURE_ADDRESS_WRAP;
    descriptor.AddressV = mirror ? D3D11_TEXTURE_ADDRESS_MIRROR : D3D11_TEXTURE_ADDRESS_WRAP;
    descriptor.AddressW = mirror ? D3D11_TEXTURE_ADDRESS_MIRROR : D3D11_TEXTURE_ADDRESS_WRAP;

    this->gpu_sampler_state::gpu_sampler_state(&descriptor);
}

void kl::gpu_sampler_state::bind_for_pixel_shader(const UINT slot) const
{
    BOUND_GPU->context()->PSSetSamplers(slot, 1, &child_object_);
}

void kl::gpu_sampler_state::bind_for_compute_shader(const UINT slot) const
{
    BOUND_GPU->context()->CSSetSamplers(slot, 1, &child_object_);
}

void kl::gpu_sampler_state::unbind_for_pixel_shader() const
{
    BOUND_GPU->context()->PSSetSamplers(0, 0, nullptr);
}

void kl::gpu_sampler_state::unbind_for_compute_shader() const
{
    BOUND_GPU->context()->CSSetSamplers(0, 0, nullptr);
}
