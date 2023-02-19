#pragma once

#include "graphics/shaders/gpu_vertex_shader.h"
#include "graphics/shaders/gpu_pixel_shader.h"


namespace kl {
    class gpu_shaders : public gpu_child<dx::unknown>
    {
        ref<gpu_vertex_shader> vertex_shader_ = nullptr;
        ref<gpu_pixel_shader> pixel_shader_ = nullptr;

        gpu_shaders(const std::string& source);

    public:
        static ref<gpu_shaders> make(const std::string& source);

        operator bool() const;

        void bind() const;
        static void unbind();
    };
}
