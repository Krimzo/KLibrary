#pragma once

#include "graphics/gpu/gpu.h"


namespace kl {
    class gpu_shader_data
    {
        dx::shader_blob data_ = nullptr;
        dx::shader_blob error_ = nullptr;

        gpu_shader_data(char shader_type, const std::string& source);

    public:
        static ref<gpu_shader_data> make(char shader_type, const std::string& source);

        virtual ~gpu_shader_data();

        bool has_data() const;
        bool has_error() const;

        void* get_data() const;
        std::string get_error() const;

        SIZE_T data_size() const;
    };
}
