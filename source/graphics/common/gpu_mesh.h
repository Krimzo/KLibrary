#pragma once

#include "graphics/buffers/gpu_buffer.h"


namespace kl {
    using mesh_data = std::vector<vertex>;

    enum class mesh_draw_type
    {
        points = D3D_PRIMITIVE_TOPOLOGY_POINTLIST,
        lines = D3D_PRIMITIVE_TOPOLOGY_LINELIST,
        triangles = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
    };

    class gpu_mesh : public gpu_child<dx::unknown>
    {
        ref<gpu_buffer> buffer_ = nullptr;
        UINT vertex_count_ = 0;

        gpu_mesh(const mesh_data& vertices);
        gpu_mesh(const std::string& filepath, bool flip_z);

    public:
        static ref<gpu_mesh> make(const mesh_data& vertices);
        static ref<gpu_mesh> make(const std::string& filepath, bool flip_z);

        operator bool() const;

        UINT vertex_count() const;

        void bind() const;
        void unbind() const;

        void draw() const;
        void draw(mesh_draw_type draw_type) const;
        void draw(D3D_PRIMITIVE_TOPOLOGY draw_type, UINT vertex_count, UINT start_index) const;

        static ref<gpu_mesh> make_screen();
        static ref<gpu_mesh> make_plane(float size, size_t num_of_points);
    };
}
