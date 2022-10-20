#include "gpu/gpu.h"

#include "utility/console.h"
#include "utility/strings.h"


kl::dx::access_view kl::gpu::new_access_view(const dx::resource resource, const dx::access_view_descriptor* descriptor)
{
    dx::access_view view = nullptr;

    if (const long result = device_->CreateUnorderedAccessView(resource, descriptor, &view); warning(
        !view, format("Failed to create unordered access view. Result: 0x", std::hex, result))) {
        return nullptr;
    }

    children_.insert(view);

    return view;
}

void kl::gpu::bind_compute_access_view(const dx::access_view view, const uint32_t slot, const uint32_t* initial_counts) const
{
    context_->CSSetUnorderedAccessViews(slot, 1, &view, initial_counts);
}
