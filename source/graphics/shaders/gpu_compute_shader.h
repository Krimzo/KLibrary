#pragma once

#include "graphics/gpu/gpu.h"


namespace kl {
    class gpu_compute_shader : public gpu_child<dx::compute_shader>
    {
        gpu_compute_shader(const std::string& source);

    public:
        static ref<gpu_compute_shader> make(const std::string& source);

        void bind() const;
        void unbind() const;

        void dispatch(const int3& size) const;
        void execute(const int3& size) const;
    };
}
