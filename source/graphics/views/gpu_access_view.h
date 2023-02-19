#pragma once

#include "graphics/gpu/gpu.h"


namespace kl {
    class gpu_access_view : public gpu_child<dx::access_view>
    {
        gpu_access_view(const dx::resource resource, const dx::access_view_descriptor* descriptor);

    public:
        static ref<gpu_access_view> make(const dx::resource resource, const dx::access_view_descriptor* descriptor);

        void bind(UINT slot, const UINT* initial_counts = nullptr) const;
        static void unbind(UINT slot);
    };
}
