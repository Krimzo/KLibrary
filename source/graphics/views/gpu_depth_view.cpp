#include "graphics/views/gpu_depth_view.h"

#include "utility/utility.h"


// Makers
kl::ref<kl::gpu_depth_view> kl::gpu_depth_view::make(const dx::resource resource, const dx::depth_view_descriptor* descriptor)
{
    return ref<gpu_depth_view>(new gpu_depth_view(resource, descriptor));
}

// Class
kl::gpu_depth_view::gpu_depth_view(const dx::resource resource, const dx::depth_view_descriptor* descriptor)
{
    const long result = BOUND_GPU->device()->CreateDepthStencilView(resource, descriptor, &child_object_);
    warning_check(!child_object_, format("Failed to create depth view. Result: 0x", std::hex, result));
}

void kl::gpu_depth_view::bind(dx::target_view target_view) const
{
    const dx::target_view target_views[1] = { target_view };
    BOUND_GPU->context()->OMSetRenderTargets(1, target_views, child_object_);
}

void kl::gpu_depth_view::unbind_all()
{
    BOUND_GPU->context()->OMSetRenderTargets(0, nullptr, nullptr);
}

void kl::gpu_depth_view::clear(const float depth, const UINT8 stencil) const
{
    if (!child_object_) { return; }
    BOUND_GPU->context()->ClearDepthStencilView(child_object_, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, stencil);
}
