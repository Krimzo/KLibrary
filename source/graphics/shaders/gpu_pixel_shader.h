#pragma once

#include "graphics/gpu/gpu.h"


namespace kl {
    class gpu_pixel_shader : public gpu_child<dx::pixel_shader>
    {
        gpu_pixel_shader(const std::string& source);

    public:
        static ref<gpu_pixel_shader> make(const std::string& source);

        void bind() const;
        static void unbind();
    };
}
