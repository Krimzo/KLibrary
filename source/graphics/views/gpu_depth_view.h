#pragma once

#include "graphics/gpu/gpu.h"


namespace kl {
    class gpu_depth_view : public gpu_child<dx::depth_view>
    {
        gpu_depth_view(const dx::resource resource, const dx::depth_view_descriptor* descriptor);

    public:
        static ref<gpu_depth_view> make(const dx::resource resource, const dx::depth_view_descriptor* descriptor);

        void bind(dx::target_view target_view) const;
        static void unbind_all();

        void clear(float depth, UINT8 stencil) const;
    };
}
