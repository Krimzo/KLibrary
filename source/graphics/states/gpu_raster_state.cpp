#include "graphics/states/gpu_raster_state.h"

#include "utility/utility.h"


// Makers
kl::ref<kl::gpu_raster_state> kl::gpu_raster_state::make(const dx::raster_state_descriptor* descriptor)
{
    return ref<gpu_raster_state>(new gpu_raster_state(descriptor));
}

kl::ref<kl::gpu_raster_state> kl::gpu_raster_state::make(bool wireframe, bool cull, bool cull_back)
{
    return ref<gpu_raster_state>(new gpu_raster_state(wireframe, cull, cull_back));
}

// Class
kl::gpu_raster_state::gpu_raster_state(const dx::raster_state_descriptor* descriptor)
{
    const long result = BOUND_GPU->device()->CreateRasterizerState(descriptor, &child_object_);
    warning_check(!child_object_, format("Failed to create rasterizer state. Result: 0x", std::hex, result));
}

kl::gpu_raster_state::gpu_raster_state(const bool wireframe, const bool cull, const bool cull_back)
{
    dx::raster_state_descriptor descriptor = {};
    descriptor.FillMode = wireframe ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID;
    descriptor.CullMode = cull ? (cull_back ? D3D11_CULL_BACK : D3D11_CULL_FRONT) : D3D11_CULL_NONE;
    descriptor.FrontCounterClockwise = true;
    descriptor.MultisampleEnable = true;
    descriptor.AntialiasedLineEnable = true;

    this->gpu_raster_state::gpu_raster_state(&descriptor);
}

void kl::gpu_raster_state::bind() const
{
    BOUND_GPU->context()->RSSetState(child_object_);
}

void kl::gpu_raster_state::unbind() const
{
    BOUND_GPU->context()->RSSetState(nullptr);
}
