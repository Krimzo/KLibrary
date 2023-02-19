#pragma once

#include "graphics/gpu/gpu.h"


namespace kl {
    class gpu_shader_view : public gpu_child<dx::shader_view>
    {
        gpu_shader_view(const dx::resource resource, const dx::shader_view_descriptor* descriptor);

    public:
        static ref<gpu_shader_view> make(const dx::resource resource, const dx::shader_view_descriptor* descriptor);

        void bind_for_pixel_shader(UINT slot) const;
        void bind_for_compute_shader(UINT slot) const;

        static void unbind_for_pixel_shader(UINT slot);
        static void unbind_for_compute_shader(UINT slot);
    };
}
