#pragma once

#include "graphics/buffers/gpu_buffer.h"


namespace kl {
    class gpu_const_buffer : public gpu_child<dx::unknown>
    {
        ref<gpu_buffer> buffer_ = nullptr;

        gpu_const_buffer(UINT byte_size);

    public:
        static ref<gpu_const_buffer> make(UINT byte_size);

        operator bool() const;

        UINT get_size() const;

        void bind_for_vertex_shader(UINT slot) const;
        void bind_for_geometry_shader(UINT slot) const;
        void bind_for_pixel_shader(UINT slot) const;
        void bind_for_compute_shader(UINT slot) const;

        static void unbind_for_vertex_shader(UINT slot);
        static void unbind_for_geometry_shader(UINT slot);
        static void unbind_for_pixel_shader(UINT slot);
        static void unbind_for_compute_shader(UINT slot);

        void set_data(const void* data, UINT byte_size) const;

        template<typename T>
        void set_data(const T& object) const
        {
            set_data(&object, sizeof(T));
        }
    };
}
