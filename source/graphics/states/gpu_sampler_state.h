#pragma once

#include "graphics/gpu/gpu.h"


namespace kl {
    class gpu_sampler_state : public gpu_child<dx::sampler_state>
    {
        gpu_sampler_state(const dx::sampler_state_descriptor* descriptor);
        gpu_sampler_state(bool linear, bool mirror);

    public:
        static ref<gpu_sampler_state> make(const dx::sampler_state_descriptor* descriptor);
        static ref<gpu_sampler_state> make(bool linear, bool mirror);

        void bind_for_pixel_shader(UINT slot) const;
        void bind_for_compute_shader(UINT slot) const;

        void unbind_for_pixel_shader() const;
        void unbind_for_compute_shader() const;
    };
}
