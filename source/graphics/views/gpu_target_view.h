#pragma once

#include "graphics/gpu/gpu.h"


namespace kl {
    class gpu_target_view : public gpu_child<dx::target_view>
    {
        gpu_target_view(const dx::resource resource, const dx::target_view_descriptor* descriptor);

    public:
        static ref<gpu_target_view> make(const dx::resource resource, const dx::target_view_descriptor* descriptor);

        void bind(dx::depth_view depth_view) const;
        void unbind_all() const;

        void clear(const float4& color) const;
    };
}
