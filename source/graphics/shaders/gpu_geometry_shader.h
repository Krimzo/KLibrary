#pragma once

#include "graphics/gpu/gpu.h"


namespace kl {
    class gpu_geometry_shader : public gpu_child<dx::geometry_shader>
    {
        gpu_geometry_shader(const std::string& source);

    public:
        static ref<gpu_geometry_shader> make(const std::string& source);

        void bind() const;
        static void unbind();
    };
}
