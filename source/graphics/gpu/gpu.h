#pragma once

#include "graphics/gpu/gpu_child.h"
#include "math/math.h"


namespace kl {
    enum class gpu_creation_type
    {
        none = 0,
        render = 1,
        compute = 2,
    };

    class gpu
    {
        gpu_creation_type creation_type_ = gpu_creation_type::none;

        dx::device device_ = nullptr;
        dx::context context_ = nullptr;
        dx::chain chain_ = nullptr;

        dx::target_view render_buffer_ = nullptr;
        dx::depth_view depth_buffer_ = nullptr;

        gpu();
        gpu(HWND window);

    public:
        static ref<gpu> make();
        static ref<gpu> make(HWND window);

        virtual ~gpu();

        gpu(const gpu&) = delete;
        gpu(const gpu&&) = delete;

        void operator=(const gpu&) = delete;
        void operator=(const gpu&&) = delete;

        dx::device device() const;
        dx::context context() const;
        dx::chain chain() const;

        gpu_creation_type get_type() const;

        void set_viewport(const int2& size) const;
        void set_viewport(const int2& position, const int2& size) const;

        void unbind_all_targets() const;
        void bind_internal_targets() const;
        void bind_targets(const std::vector<dx::target_view>& targets, dx::depth_view depth_view = nullptr) const;
        void bind_targets_with_internal(const std::vector<dx::target_view>& additional_targets, dx::depth_view depth_view = nullptr) const;

        void resize_internal(const int2& size);

        void clear_internal_color(const float4& color) const;
        void clear_internal_depth(float depth, UINT8 stencil) const;
        void clear_internal() const;

        void swap_buffers(bool v_sync) const;

        void copy_resource(dx::resource destination, dx::resource source) const;
        void read_from_resource(void* cpu_buffer, dx::resource cpu_read_resource, int byte_size) const;
        void write_to_resource(dx::resource cpu_write_resource, const void* data, int byte_size, bool discard = true) const;
    };

    inline ref<gpu> BOUND_GPU = nullptr;
}
