#pragma once

#include "graphics/gpu/gpu.h"
#include "media/media.h"


namespace kl {
    enum class gpu_texture_creation_type
    {
        none = 0,
        normal = 1,
        cube = 2,
        staging = 3,
    };

    class gpu_texture : public gpu_child<dx::texture>
    {
        gpu_texture(const dx::texture_descriptor* descriptor, const dx::subresource_descriptor* subresource_data);
        gpu_texture(const image& image, bool has_unordered_access, bool is_target);
        gpu_texture(const image& front, const image& back, const image& left, const image& right, const image& top, const image& bottom);
        gpu_texture(const dx::texture texture, const int2& size);

    public:
        const gpu_texture_creation_type creation_type = gpu_texture_creation_type::none;

        static ref<gpu_texture> make(const dx::texture_descriptor* descriptor, const dx::subresource_descriptor* subresource_data);
        static ref<gpu_texture> make(const image& image, bool has_unordered_access, bool is_target);
        static ref<gpu_texture> make(const image& front, const image& back, const image& left, const image& right, const image& top, const image& bottom);
        static ref<gpu_texture> make(const dx::texture texture, const int2& size);

        static dx::texture get_back_buffer(dx::chain chain);
    };
}
