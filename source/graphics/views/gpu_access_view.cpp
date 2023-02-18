#include "graphics/views/gpu_access_view.h"

#include "utility/utility.h"


// Makers
kl::ref<kl::gpu_access_view> kl::gpu_access_view::make(const dx::resource resource, const dx::access_view_descriptor* descriptor)
{
    return ref<gpu_access_view>(new gpu_access_view(resource, descriptor));
}

// Class
kl::gpu_access_view::gpu_access_view(const dx::resource resource, const dx::access_view_descriptor* descriptor)
{
    const long result = BOUND_GPU->device()->CreateUnorderedAccessView(resource, descriptor, &child_object_);
    warning_check(!child_object_, format("Failed to create unordered access view. Result: 0x", std::hex, result));
}

void kl::gpu_access_view::bind(const UINT slot, const UINT* initial_counts) const
{
    BOUND_GPU->context()->CSSetUnorderedAccessViews(slot, 1, &child_object_, initial_counts);
}

void kl::gpu_access_view::unbind() const
{
    BOUND_GPU->context()->CSSetUnorderedAccessViews(0, 0, nullptr, nullptr);
}
