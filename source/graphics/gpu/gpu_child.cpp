#include "graphics/gpu/gpu_child.h"


void kl::safe_release(IUnknown*& unknown)
{
    if (!unknown) { return; }
    unknown->Release();
    unknown = nullptr;
}

void kl::safe_release(IUnknown* const& unknown)
{
    if (!unknown) { return; }
    unknown->Release();
}
