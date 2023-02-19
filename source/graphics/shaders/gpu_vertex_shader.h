#pragma once

#include "graphics/gpu/gpu.h"


namespace kl {
    class gpu_vertex_shader : public gpu_child<dx::vertex_shader>
    {
        dx::layout layout_ = nullptr;

        gpu_vertex_shader(const std::string& source, const std::vector<dx::layout_descriptor>& descriptors);

    public:
        static ref<gpu_vertex_shader> make(const std::string& source, const std::vector<dx::layout_descriptor>& descriptors = {});

        virtual ~gpu_vertex_shader();

        void bind() const;
        static void unbind();
    };
}
