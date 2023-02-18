#include "graphics/common/gpu_mesh.h"

#include "memory/memory.h"


// Makers
kl::ref<kl::gpu_mesh> kl::gpu_mesh::make(const mesh_data& vertices)
{
    return ref<gpu_mesh>(new gpu_mesh(vertices));
}

kl::ref<kl::gpu_mesh> kl::gpu_mesh::make(const std::string& filepath, const bool flip_z)
{
    return ref<gpu_mesh>(new gpu_mesh(filepath, flip_z));
}

// Class
kl::gpu_mesh::gpu_mesh(const mesh_data& vertices)
{
    dx::buffer_descriptor descriptor = {};
    descriptor.ByteWidth = UINT(vertices.size() * sizeof(vertex));
    descriptor.Usage = D3D11_USAGE_DEFAULT;
    descriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    dx::subresource_descriptor subresource_data = {};
    subresource_data.pSysMem = vertices.data();

    buffer_ = gpu_buffer::make(&descriptor, &subresource_data);
    vertex_count_ = (UINT) vertices.size();
}

kl::gpu_mesh::gpu_mesh(const std::string& filepath, const bool flip_z)
    : gpu_mesh(files::parse_mesh(filepath, flip_z))
{}

kl::gpu_mesh::operator bool() const
{
    return (bool) buffer_;
}

UINT kl::gpu_mesh::vertex_count() const
{
    return vertex_count_;
}

void kl::gpu_mesh::bind() const
{
    static const UINT stride = sizeof(vertex);
    static const UINT offset = 0;

    if (!buffer_) { return; }
    BOUND_GPU->context()->IASetVertexBuffers(0, 1, *buffer_, &stride, &offset);
}

void kl::gpu_mesh::unbind() const
{
    BOUND_GPU->context()->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
}

void kl::gpu_mesh::draw() const
{
    draw(mesh_draw_type::triangles);
}

void kl::gpu_mesh::draw(mesh_draw_type draw_type) const
{
    draw((D3D_PRIMITIVE_TOPOLOGY) draw_type, 0, 0);
}

void kl::gpu_mesh::draw(D3D_PRIMITIVE_TOPOLOGY draw_type, UINT vertex_count, UINT start_index) const
{
    if (!buffer_) { return; }
    BOUND_GPU->context()->IASetPrimitiveTopology(draw_type);

    if (vertex_count == 0 || vertex_count > vertex_count_) {
        vertex_count = vertex_count_;
    }

    bind();
    BOUND_GPU->context()->Draw(vertex_count, start_index);
}

// Static
kl::ref<kl::gpu_mesh> kl::gpu_mesh::make_screen()
{
    return gpu_mesh::make({
        vertex({ -1.0f, -1.0f, 0.5f }), vertex({ -1.0f, 1.0f, 0.5f }), vertex({ 1.0f, 1.0f, 0.5f }),
        vertex({ 1.0f, 1.0f, 0.5f }), vertex({ 1.0f, -1.0f, 0.5f }), vertex({ -1.0f, -1.0f, 0.5f })
        });
}

kl::ref<kl::gpu_mesh> kl::gpu_mesh::make_plane(const float size, size_t num_of_points)
{
    // Count fix
    const float increment = size / num_of_points;
    num_of_points += 1;

    // Generating points
    std::vector<float2> points = {};
    points.reserve(num_of_points * num_of_points);
    for (int x = 0; x < num_of_points; x++) {
        for (int z = 0; z < num_of_points; z++) {
            points.emplace_back(
                -size * 0.5f + increment * x,
                -size * 0.5f + increment * z
            );
        }
    }

    // Generating triangles
    std::vector<vertex> vertices = {};
    vertices.reserve((num_of_points - 1) * (num_of_points - 1) * 6);
    for (size_t x = 0; x < num_of_points - 1; x++) {
        for (size_t z = 0; z < num_of_points - 1; z++) {
            const float2& xz_point = points[z * num_of_points + x];
            const float2& xz_plus_point = points[(z + 1) * num_of_points + (x + 1)];

            vertices.push_back(vertex({ xz_plus_point.x, 0, xz_plus_point.y }));
            vertices.push_back(vertex({ xz_plus_point.x, 0, xz_point.y }));
            vertices.push_back(vertex({ xz_point.x, 0, xz_point.y }));

            vertices.push_back(vertex({ xz_point.x, 0, xz_point.y }));
            vertices.push_back(vertex({ xz_point.x, 0, xz_plus_point.y }));
            vertices.push_back(vertex({ xz_plus_point.x, 0, xz_plus_point.y }));
        }
    }

    return gpu_mesh::make(vertices);
}
