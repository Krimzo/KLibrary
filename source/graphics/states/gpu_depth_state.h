#pragma once

#include "graphics/gpu/gpu.h"


namespace kl {
    class gpu_depth_state : public gpu_child<dx::depth_state>
    {
        gpu_depth_state(const dx::depth_state_descriptor* descriptor);
        gpu_depth_state(bool depth, bool stencil, bool mask);

    public:
        static ref<gpu_depth_state> make(const dx::depth_state_descriptor* descriptor);
        static ref<gpu_depth_state> make(bool depth, bool stencil, bool mask);

        void bind() const;
        void bind(UINT stencil_ref) const;

        void unbind() const;
    };
}
