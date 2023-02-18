#pragma once

#include "graphics/gpu/gpu.h"


namespace kl {
    enum class gpu_buffer_creation_type
    {
        none = 0,
        normal = 1,
        structured = 2,
        staging = 3,
    };

    class gpu_buffer : public gpu_child<dx::buffer>
    {
        gpu_buffer(const dx::buffer_descriptor* descriptor, const dx::subresource_descriptor* subresource_data);
        gpu_buffer(const void* data, UINT element_count, UINT element_size, bool has_unordered_access, bool cpu_read);
        gpu_buffer(const dx::buffer buffer, UINT byte_size);

    public:
        const gpu_buffer_creation_type creation_type = gpu_buffer_creation_type::none;

        static ref<gpu_buffer> make(const dx::buffer_descriptor* descriptor, const dx::subresource_descriptor* subresource_data);
        static ref<gpu_buffer> make(const void* data, UINT element_count, UINT element_size, bool has_unordered_access, bool cpu_read);
        static ref<gpu_buffer> make(const dx::buffer buffer, UINT byte_size);

        UINT get_size() const;
    };
}
