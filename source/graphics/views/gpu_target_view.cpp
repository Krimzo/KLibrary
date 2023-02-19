#include "graphics/views/gpu_target_view.h"

#include "utility/utility.h"


// Makers
kl::ref<kl::gpu_target_view> kl::gpu_target_view::make(const dx::resource resource, const dx::target_view_descriptor* descriptor)
{
    return ref<gpu_target_view>(new gpu_target_view(resource, descriptor));
}

// Class
kl::gpu_target_view::gpu_target_view(const dx::resource resource, const dx::target_view_descriptor* descriptor)
{
    const long result = BOUND_GPU->device()->CreateRenderTargetView(resource, descriptor, &child_object_);
    warning_check(!child_object_, format("Failed to create render target view. Result: 0x", std::hex, result));
}

void kl::gpu_target_view::bind(dx::depth_view depth_view) const
{
    const dx::target_view target_views[1] = { child_object_ };
    BOUND_GPU->context()->OMSetRenderTargets(1, target_views, depth_view);
}

void kl::gpu_target_view::unbind_all()
{
    BOUND_GPU->context()->OMSetRenderTargets(0, nullptr, nullptr);
}

void kl::gpu_target_view::clear(const float4& color) const
{
    if (!child_object_) { return; }
    BOUND_GPU->context()->ClearRenderTargetView(child_object_, color);
}
