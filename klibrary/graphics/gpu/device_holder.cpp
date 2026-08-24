#include "klibrary.h"

kl::dx::RasterState kl::DeviceHolder::create_raster_state(dx::RasterStateDescriptor const* descriptor) const
{
    dx::RasterState state;
    m_device->CreateRasterizerState(descriptor, &state);
    return state;
}

kl::dx::RasterState kl::DeviceHolder::create_raster_state(bool wireframe, bool cull, bool cull_back) const
{
    dx::RasterStateDescriptor descriptor{};
    descriptor.FillMode = wireframe ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID;
    descriptor.CullMode = cull ? (cull_back ? D3D11_CULL_BACK : D3D11_CULL_FRONT) : D3D11_CULL_NONE;
    descriptor.MultisampleEnable = true;
    descriptor.AntialiasedLineEnable = true;
    descriptor.DepthClipEnable = true;
    return create_raster_state(&descriptor);
}

kl::dx::DepthState kl::DeviceHolder::create_depth_state(dx::DepthStateDescriptor const* descriptor) const
{
    dx::DepthState state;
    m_device->CreateDepthStencilState(descriptor, &state);
    return state;
}

kl::dx::DepthState kl::DeviceHolder::create_depth_state(bool depth, bool stencil, bool mask) const
{
    dx::DepthStateDescriptor descriptor{};
    if (depth)
    {
        descriptor.DepthEnable = true;
        descriptor.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        descriptor.DepthFunc = D3D11_COMPARISON_LESS;
    }
    if (stencil)
    {
        descriptor.StencilEnable = true;
        descriptor.StencilReadMask = 0xFF;
        descriptor.StencilWriteMask = 0xFF;
        if (mask)
        {
            descriptor.DepthEnable = false;
            descriptor.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
            descriptor.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
            descriptor.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
            descriptor.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
            descriptor.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
            descriptor.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
            descriptor.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
            descriptor.BackFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
        }
        else
        {
            descriptor.FrontFace.StencilFailOp = D3D11_STENCIL_OP_REPLACE;
            descriptor.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_REPLACE;
            descriptor.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
            descriptor.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
            descriptor.BackFace.StencilFailOp = D3D11_STENCIL_OP_REPLACE;
            descriptor.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_REPLACE;
            descriptor.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
            descriptor.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
        }
    }
    return create_depth_state(&descriptor);
}

kl::dx::SamplerState kl::DeviceHolder::create_sampler_state(dx::SamplerStateDescriptor const* descriptor) const
{
    dx::SamplerState state;
    m_device->CreateSamplerState(descriptor, &state);
    return state;
}

kl::dx::SamplerState kl::DeviceHolder::create_sampler_state(bool linear, bool mirror) const
{
    dx::SamplerStateDescriptor descriptor{};
    descriptor.Filter = linear ? D3D11_FILTER_MIN_MAG_MIP_LINEAR : D3D11_FILTER_MIN_MAG_MIP_POINT;
    descriptor.AddressU = mirror ? D3D11_TEXTURE_ADDRESS_MIRROR : D3D11_TEXTURE_ADDRESS_WRAP;
    descriptor.AddressV = descriptor.AddressU;
    descriptor.AddressW = descriptor.AddressU;
    return create_sampler_state(&descriptor);
}

kl::dx::BlendState kl::DeviceHolder::create_blend_state(dx::BlendStateDescriptor const* descriptor) const
{
    dx::BlendState state;
    m_device->CreateBlendState(descriptor, &state);
    return state;
}

kl::dx::BlendState kl::DeviceHolder::create_blend_state(bool transparency) const
{
    dx::BlendStateDescriptor descriptor{};
    descriptor.RenderTarget[0].BlendEnable = transparency;
    descriptor.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    descriptor.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    descriptor.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    descriptor.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
    descriptor.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
    descriptor.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    descriptor.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    return create_blend_state(&descriptor);
}

kl::dx::Buffer kl::DeviceHolder::create_buffer(dx::BufferDescriptor const* descriptor,
                                               dx::SubresourceDescriptor const* subresource_data) const
{
    dx::Buffer buffer;
    m_device->CreateBuffer(descriptor, subresource_data, &buffer);
    return buffer;
}

kl::dx::Buffer kl::DeviceHolder::create_vertex_buffer(void const* data, UINT byte_size) const
{
    dx::BufferDescriptor descriptor{};
    descriptor.ByteWidth = byte_size;
    descriptor.Usage = D3D11_USAGE_IMMUTABLE;
    descriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    dx::SubresourceDescriptor subresource_data{};
    subresource_data.pSysMem = data;
    return create_buffer(&descriptor, &subresource_data);
}

kl::dx::Buffer kl::DeviceHolder::create_vertex_buffer(std::vector<Vertex> const& vertices) const
{
    return create_vertex_buffer(vertices.data(), UINT(vertices.size() * sizeof(Vertex)));
}

kl::dx::Buffer kl::DeviceHolder::create_vertex_buffer(std::vector<Triangle> const& triangles) const
{
    return create_vertex_buffer(triangles.data(), UINT(triangles.size() * sizeof(Triangle)));
}

kl::dx::Buffer kl::DeviceHolder::create_vertex_buffer(std::string_view const& filepath, bool flip_z) const
{
    return create_vertex_buffer(parse_obj_file(filepath, flip_z));
}

kl::dx::Buffer kl::DeviceHolder::create_index_buffer(uint32_t const* data, UINT element_count) const
{
    dx::BufferDescriptor descriptor{};
    descriptor.ByteWidth = element_count * sizeof(uint32_t);
    descriptor.Usage = D3D11_USAGE_IMMUTABLE;
    descriptor.BindFlags = D3D11_BIND_INDEX_BUFFER;
    dx::SubresourceDescriptor subresource_data{};
    subresource_data.pSysMem = data;
    return create_buffer(&descriptor, &subresource_data);
}

kl::dx::Buffer kl::DeviceHolder::create_index_buffer(std::vector<uint32_t> const& indices) const
{
    return create_index_buffer(indices.data(), (UINT)indices.size());
}

kl::dx::Buffer kl::DeviceHolder::create_const_buffer(UINT byte_size) const
{
    if (byte_size % 16 != 0)
        return {};
    dx::BufferDescriptor descriptor{};
    descriptor.ByteWidth = byte_size;
    descriptor.Usage = D3D11_USAGE_DYNAMIC;
    descriptor.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    descriptor.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    return create_buffer(&descriptor, nullptr);
}

kl::dx::Buffer kl::DeviceHolder::create_structured_buffer(void const* data, UINT element_count, UINT element_size,
                                                          bool has_unordered_access, bool cpu_read) const
{
    dx::BufferDescriptor descriptor{};
    descriptor.Usage = D3D11_USAGE_DEFAULT;
    descriptor.BindFlags = D3D11_BIND_SHADER_RESOURCE | (has_unordered_access ? D3D11_BIND_UNORDERED_ACCESS : NULL);
    descriptor.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    descriptor.CPUAccessFlags = cpu_read ? D3D11_CPU_ACCESS_READ : NULL;
    descriptor.StructureByteStride = element_size;
    descriptor.ByteWidth = element_count * element_size;
    dx::SubresourceDescriptor subresource{};
    subresource.pSysMem = data;
    return create_buffer(&descriptor, &subresource);
}

kl::dx::Buffer kl::DeviceHolder::create_staging_buffer(dx::Buffer const& buffer, UINT byte_size) const
{
    dx::BufferDescriptor descriptor{};
    buffer->GetDesc(&descriptor);
    descriptor.Usage = D3D11_USAGE_STAGING;
    descriptor.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    descriptor.ByteWidth = byte_size ? byte_size : descriptor.ByteWidth;
    return create_buffer(&descriptor, nullptr);
}

std::vector<kl::Triangle> kl::DeviceHolder::generate_screen_mesh()
{
    std::vector<Triangle> triangles;
    triangles.emplace_back(Vertex{{-1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
                           Vertex{{1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},
                           Vertex{{1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}});
    triangles.emplace_back(Vertex{{-1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
                           Vertex{{1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},
                           Vertex{{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}});
    return triangles;
}

std::vector<kl::Triangle> kl::DeviceHolder::generate_plane_mesh(float size, int complexity)
{
    if (complexity < 2)
        return {};
    std::vector<Triangle> triangles;
    triangles.reserve(size_t(complexity - 1) * size_t(complexity - 1) * 2);
    const float incr = size / (complexity - 1);
    for (int x = 0; x < complexity - 1; x++)
    {
        for (int z = 0; z < complexity - 1; z++)
        {
            float3 lb = {x * incr, 0.0f, z * incr};
            float3 lt = lb + float3{0.0f, 0.0f, incr};
            float3 rb = lb + float3{incr, 0.0f, 0.0f};
            float3 rt = lb + float3{incr, 0.0f, incr};
            triangles.emplace_back(
                Vertex{
                    lb, {0.0f, 1.0f, 0.0f}, {float(x + 0) / (complexity - 1), 1.0f - float(z + 0) / (complexity - 1)}},
                Vertex{
                    lt, {0.0f, 1.0f, 0.0f}, {float(x + 0) / (complexity - 1), 1.0f - float(z + 1) / (complexity - 1)}},
                Vertex{
                    rt, {0.0f, 1.0f, 0.0f}, {float(x + 1) / (complexity - 1), 1.0f - float(z + 1) / (complexity - 1)}});
            triangles.emplace_back(
                Vertex{
                    lb, {0.0f, 1.0f, 0.0f}, {float(x + 0) / (complexity - 1), 1.0f - float(z + 0) / (complexity - 1)}},
                Vertex{
                    rt, {0.0f, 1.0f, 0.0f}, {float(x + 1) / (complexity - 1), 1.0f - float(z + 1) / (complexity - 1)}},
                Vertex{
                    rb, {0.0f, 1.0f, 0.0f}, {float(x + 1) / (complexity - 1), 1.0f - float(z + 0) / (complexity - 1)}});
        }
    }
    return triangles;
}

std::vector<kl::Triangle> kl::DeviceHolder::generate_cube_mesh(float size)
{
    static constexpr Triangle face[2] = {
        Triangle{
            {{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
            {{0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},
            {{0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},
        },
        Triangle{
            {{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
            {{0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},
            {{-0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},
        },
    };
    static constexpr auto mul_vr = [](Vertex const& vertex, float3 const& other) -> Vertex {
        Vertex result;
        result.position = vertex.position * other;
        result.normal = vertex.normal;
        result.uv = vertex.uv;
        return result;
    };
    static constexpr auto rotate_vr = [](Vertex const& vertex, float3 const& axis, float angle) -> Vertex {
        Vertex result;
        result.position = rotate(vertex.position, axis, angle);
        result.normal = rotate(vertex.normal, axis, angle);
        result.uv = vertex.uv;
        return result;
    };
    static constexpr auto mul_tr = [](Triangle const& triangle, float3 const& other) -> Triangle {
        Triangle result;
        result.a = mul_vr(triangle.a, other);
        result.b = mul_vr(triangle.b, other);
        result.c = mul_vr(triangle.c, other);
        return result;
    };
    static constexpr auto rotate_tr = [](Triangle const& triangle, float3 const& axis, float angle) -> Triangle {
        Triangle result;
        result.a = rotate_vr(triangle.a, axis, angle);
        result.b = rotate_vr(triangle.b, axis, angle);
        result.c = rotate_vr(triangle.c, axis, angle);
        return result;
    };

    std::vector<Triangle> triangles;
    for (Triangle const& triangle : face)
    {
        triangles.push_back(mul_tr(triangle, float3{size}));
        triangles.push_back(rotate_tr(triangles.back(), {1.0f, 0.0f, 0.0f}, 90.0f));
        triangles.push_back(rotate_tr(triangles.back(), {0.0f, 1.0f, 0.0f}, 90.0f));
        triangles.push_back(rotate_tr(triangles.back(), {0.0f, 1.0f, 0.0f}, 90.0f));
        triangles.push_back(rotate_tr(triangles.back(), {0.0f, 1.0f, 0.0f}, 90.0f));
        triangles.push_back(
            rotate_tr(rotate_tr(triangles.back(), {0.0f, 1.0f, 0.0f}, 90.0f), {1.0f, 0.0f, 0.0f}, 90.0f));
    }
    return triangles;
}

std::vector<kl::Triangle> kl::DeviceHolder::generate_sphere_mesh(float radius, int h_slices, int v_slices, bool smooth)
{
    const auto update_normal = [&](Triangle& triangle) -> Triangle& {
        const float3 a_norm = normalize(triangle.a.position);
        const float3 b_norm = normalize(triangle.b.position);
        const float3 c_norm = normalize(triangle.c.position);
        if (smooth)
        {
            triangle.a.normal = a_norm;
            triangle.b.normal = b_norm;
            triangle.c.normal = c_norm;
        }
        else
        {
            float3 normal = triangle.normal();
            triangle.a.normal = normal;
            triangle.b.normal = normal;
            triangle.c.normal = normal;
        }
        return triangle;
    };
    std::vector<Triangle> triangles;
    const float3 up = {0.0f, radius, 0.0f};
    for (int x = 0; x < v_slices; x++)
    {
        for (int y = 0; y < h_slices; y++)
        {
            const float3 tl = rotate(rotate(up, {1.0f, 0.0f, 0.0f}, -180.0f / h_slices * (y + 0)), {0.0f, 1.0f, 0.0f},
                                     -360.0f / v_slices * (x + 0));
            const float3 tr = rotate(rotate(up, {1.0f, 0.0f, 0.0f}, -180.0f / h_slices * (y + 0)), {0.0f, 1.0f, 0.0f},
                                     -360.0f / v_slices * (x + 1));
            const float3 bl = rotate(rotate(up, {1.0f, 0.0f, 0.0f}, -180.0f / h_slices * (y + 1)), {0.0f, 1.0f, 0.0f},
                                     -360.0f / v_slices * (x + 0));
            const float3 br = rotate(rotate(up, {1.0f, 0.0f, 0.0f}, -180.0f / h_slices * (y + 1)), {0.0f, 1.0f, 0.0f},
                                     -360.0f / v_slices * (x + 1));
            if (y > 0)
            {
                auto& triangle = update_normal(triangles.emplace_back(tl, tr, bl));
                triangle.a.uv = {(x + 0.0f) / v_slices, (y + 0.0f) / h_slices};
                triangle.b.uv = {(x + 1.0f) / v_slices, (y + 0.0f) / h_slices};
                triangle.c.uv = {(x + 0.0f) / v_slices, (y + 1.0f) / h_slices};
            }
            if (y < h_slices - 1)
            {
                auto& triangle = update_normal(triangles.emplace_back(bl, tr, br));
                triangle.a.uv = {(x + 0.0f) / v_slices, (y + 1.0f) / h_slices};
                triangle.b.uv = {(x + 1.0f) / v_slices, (y + 0.0f) / h_slices};
                triangle.c.uv = {(x + 1.0f) / v_slices, (y + 1.0f) / h_slices};
            }
        }
    }
    return triangles;
}

std::vector<kl::Triangle> kl::DeviceHolder::generate_capsule_mesh(float radius, float height, int sectors, int rings)
{
    const auto gen_hem = [&] {
        std::vector<Triangle> triangles;
        float half_height = height * 0.5f;
        float3 top_center = float3{0.0f, half_height, 0.0f};
        for (int i = 0; i < sectors; i++)
        {
            float first_phi = (float(i) / sectors) * 2.0f * pi();
            float first_x = cos(first_phi) * radius;
            float first_z = sin(first_phi) * radius;
            float second_phi = (float(i + 1) / sectors) * 2.0f * pi();
            float second_x = cos(second_phi) * radius;
            float second_z = sin(second_phi) * radius;
            float3 first = float3{first_x, half_height, first_z};
            float3 second = float3{second_x, half_height, second_z};
            float3 first_dir = float3{0.0f, half_height + radius, 0.0f} - first;
            float3 second_dir = float3{0.0f, half_height + radius, 0.0f} - second;
            float first_len = first_dir.length();
            float second_len = second_dir.length();
            first_dir /= first_len;
            second_dir /= second_len;
            first_len /= rings;
            second_len /= rings;
            for (int j = 0; j < rings; j++)
            {
                float3 left_first =
                    normalize(first + first_dir * first_len * float(j) - top_center) * radius + top_center;
                float3 left_second =
                    normalize(first + first_dir * first_len * float(j + 1) - top_center) * radius + top_center;
                float3 right_first =
                    normalize(second + second_dir * second_len * float(j) - top_center) * radius + top_center;
                float3 right_second =
                    normalize(second + second_dir * second_len * float(j + 1) - top_center) * radius + top_center;
                triangles.emplace_back(left_first, left_second, right_second);
                triangles.emplace_back(left_first, right_second, right_first);
            }
        }
        return triangles;
    };
    const auto gen_cyl = [&] {
        std::vector<Triangle> triangles;
        float half_height = height * 0.5f;
        for (int i = 0; i < sectors; i++)
        {
            float first_phi = (float(i) / sectors) * 2.0f * pi();
            float first_x = cos(first_phi) * radius;
            float first_z = sin(first_phi) * radius;
            float second_phi = (float(i + 1) / sectors) * 2.0f * pi();
            float second_x = cos(second_phi) * radius;
            float second_z = sin(second_phi) * radius;
            triangles.emplace_back(float3{first_x, -half_height, first_z}, float3{first_x, half_height, first_z},
                                   float3{second_x, half_height, second_z});
            triangles.emplace_back(float3{first_x, -half_height, first_z}, float3{second_x, half_height, second_z},
                                   float3{second_x, -half_height, second_z});
        }
        return triangles;
    };

    std::vector<Triangle> top_hem = gen_hem();
    std::for_each(std::execution::par, top_hem.begin(), top_hem.end(),
                  [](Triangle& triangle) { std::swap(triangle.a, triangle.c); });
    std::vector<Triangle> cylinder = gen_cyl();
    std::for_each(std::execution::par, cylinder.begin(), cylinder.end(),
                  [](Triangle& triangle) { std::swap(triangle.a, triangle.c); });
    std::vector<Triangle> bottom_hem = gen_hem();
    std::for_each(std::execution::par, bottom_hem.begin(), bottom_hem.end(), [](Triangle& triangle) {
        triangle.a.position.y *= -1.0f;
        triangle.b.position.y *= -1.0f;
        triangle.c.position.y *= -1.0f;
    });

    std::vector<Triangle> triangles;
    triangles.insert(triangles.end(), top_hem.begin(), top_hem.end());
    triangles.insert(triangles.end(), cylinder.begin(), cylinder.end());
    triangles.insert(triangles.end(), bottom_hem.begin(), bottom_hem.end());
    std::for_each(std::execution::par, triangles.begin(), triangles.end(), [](Triangle& triangle) {
        std::swap(triangle.a.x, triangle.a.y);
        std::swap(triangle.b.x, triangle.b.y);
        std::swap(triangle.c.x, triangle.c.y);
        float3 normal = triangle.normal();
        triangle.a.normal = normal;
        triangle.b.normal = normal;
        triangle.c.normal = normal;
    });
    return triangles;
}

kl::dx::Buffer kl::DeviceHolder::create_screen_mesh() const
{
    return create_vertex_buffer(generate_screen_mesh());
}

kl::dx::Buffer kl::DeviceHolder::create_plane_mesh(float size, int complexity) const
{
    return create_vertex_buffer(generate_plane_mesh(size, complexity));
}

kl::dx::Buffer kl::DeviceHolder::create_cube_mesh(float size) const
{
    return create_vertex_buffer(generate_cube_mesh(size));
}

kl::dx::Buffer kl::DeviceHolder::create_sphere_mesh(float radius, int h_slices, int v_slices, bool smooth) const
{
    return create_vertex_buffer(generate_sphere_mesh(radius, h_slices, v_slices, smooth));
}

kl::dx::Buffer kl::DeviceHolder::create_capsule_mesh(float radius, float height, int sectors, int rings) const
{
    return create_vertex_buffer(generate_capsule_mesh(radius, height, sectors, rings));
}

kl::dx::Texture kl::DeviceHolder::create_texture(dx::TextureDescriptor const* descriptor,
                                                 dx::SubresourceDescriptor const* subresource_data) const
{
    dx::Texture texture;
    m_device->CreateTexture2D(descriptor, subresource_data, &texture);
    return texture;
}

kl::dx::Texture kl::DeviceHolder::create_texture(Image const& image, bool has_unordered_access, bool is_target) const
{
    dx::TextureDescriptor descriptor{};
    descriptor.Width = image.width();
    descriptor.Height = image.height();
    descriptor.MipLevels = 1;
    descriptor.ArraySize = 1;
    descriptor.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    descriptor.SampleDesc.Count = 1;
    descriptor.Usage = D3D11_USAGE_DEFAULT;
    descriptor.BindFlags = D3D11_BIND_SHADER_RESOURCE | (has_unordered_access ? D3D11_BIND_UNORDERED_ACCESS : NULL) |
                           (is_target ? D3D11_BIND_RENDER_TARGET : NULL);
    dx::SubresourceDescriptor data{};
    data.pSysMem = image.ptr();
    data.SysMemPitch = image.width() * sizeof(RGB);
    return create_texture(&descriptor, &data);
}

kl::dx::Texture kl::DeviceHolder::create_cube_texture(Image const& right, Image const& left, Image const& top,
                                                      Image const& bottom, Image const& front, Image const& back) const
{
    if (right.size() != left.size() || right.size() != top.size() || right.size() != bottom.size() ||
        right.size() != front.size() || right.size() != back.size())
        return {};

    dx::TextureDescriptor descriptor{};
    descriptor.Width = right.width();
    descriptor.Height = right.height();
    descriptor.MipLevels = 1;
    descriptor.ArraySize = 6;
    descriptor.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    descriptor.SampleDesc.Count = 1;
    descriptor.Usage = D3D11_USAGE_DEFAULT;
    descriptor.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    descriptor.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
    UINT mem_pitch = UINT(right.width() * sizeof(RGB));
    dx::SubresourceDescriptor data[6] = {
        {right.ptr(), mem_pitch, 0},  {left.ptr(), mem_pitch, 0},  {top.ptr(), mem_pitch, 0},
        {bottom.ptr(), mem_pitch, 0}, {front.ptr(), mem_pitch, 0}, {back.ptr(), mem_pitch, 0},
    };
    return create_texture(&descriptor, data);
}

kl::dx::Texture kl::DeviceHolder::create_staging_texture(dx::Texture const& texture, int2 size) const
{
    dx::TextureDescriptor descriptor{};
    texture->GetDesc(&descriptor);
    dx::TextureDescriptor staging_descriptor{};
    staging_descriptor.Width = (size.x > 0) ? size.x : descriptor.Width;
    staging_descriptor.Height = (size.y > 0) ? size.y : descriptor.Height;
    staging_descriptor.MipLevels = 1;
    staging_descriptor.ArraySize = 1;
    staging_descriptor.Format = descriptor.Format;
    staging_descriptor.SampleDesc.Count = 1;
    staging_descriptor.Usage = D3D11_USAGE_STAGING;
    staging_descriptor.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    return create_texture(&staging_descriptor, nullptr);
}

kl::dx::Texture kl::DeviceHolder::create_target_texture(int2 size) const
{
    dx::TextureDescriptor descriptor{};
    descriptor.Usage = D3D11_USAGE_DEFAULT;
    descriptor.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    descriptor.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    descriptor.Width = (UINT)size.x;
    descriptor.Height = (UINT)size.y;
    descriptor.ArraySize = 1;
    descriptor.MipLevels = 1;
    descriptor.SampleDesc.Count = 1;
    return create_texture(&descriptor, nullptr);
}

kl::dx::TargetView kl::DeviceHolder::create_target_view(dx::Resource const& resource,
                                                        dx::TargetViewDescriptor const* descriptor) const
{
    dx::TargetView view;
    m_device->CreateRenderTargetView(resource.get(), descriptor, &view);
    return view;
}

kl::dx::DepthView kl::DeviceHolder::create_depth_view(dx::Resource const& resource,
                                                      dx::DepthViewDescriptor const* descriptor) const
{
    dx::DepthView view;
    m_device->CreateDepthStencilView(resource.get(), descriptor, &view);
    return view;
}

kl::dx::ShaderView kl::DeviceHolder::create_shader_view(dx::Resource const& resource,
                                                        dx::ShaderViewDescriptor const* descriptor) const
{
    dx::ShaderView view;
    m_device->CreateShaderResourceView(resource.get(), descriptor, &view);
    return view;
}

kl::dx::AccessView kl::DeviceHolder::create_access_view(dx::Resource const& resource,
                                                        dx::AccessViewDescriptor const* descriptor) const
{
    dx::AccessView view;
    m_device->CreateUnorderedAccessView(resource.get(), descriptor, &view);
    return view;
}

kl::dx::InputLayout kl::DeviceHolder::create_input_layout(
    CompiledShader const& compiled_shader, std::initializer_list<dx::LayoutDescriptor> const& descriptors) const
{
    static constexpr dx::LayoutDescriptor default_layout_descriptors[3] = {
        {"KL_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,
         0},
        {"KL_Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"KL_UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };
    dx::LayoutDescriptor const* descriptors_ptr =
        !descriptors.empty() ? descriptors.data() : default_layout_descriptors;
    const UINT descriptors_count =
        !descriptors.empty() ? (UINT)descriptors.size() : (UINT)std::size(default_layout_descriptors);
    dx::InputLayout layout;
    m_device->CreateInputLayout(descriptors_ptr, descriptors_count, compiled_shader.data_ptr(),
                                compiled_shader.data_size(), &layout);
    return layout;
}

kl::dx::VertexShader kl::DeviceHolder::create_vertex_shader(CompiledShader const& compiled_shader) const
{
    dx::VertexShader shader;
    m_device->CreateVertexShader(compiled_shader.data_ptr(), compiled_shader.data_size(), nullptr, &shader);
    return shader;
}

kl::dx::GeometryShader kl::DeviceHolder::create_geometry_shader(CompiledShader const& compiled_shader) const
{
    dx::GeometryShader shader;
    m_device->CreateGeometryShader(compiled_shader.data_ptr(), compiled_shader.data_size(), nullptr, &shader);
    return shader;
}

kl::dx::PixelShader kl::DeviceHolder::create_pixel_shader(CompiledShader const& compiled_shader) const
{
    dx::PixelShader shader;
    m_device->CreatePixelShader(compiled_shader.data_ptr(), compiled_shader.data_size(), nullptr, &shader);
    return shader;
}

kl::dx::ComputeShader kl::DeviceHolder::create_compute_shader(CompiledShader const& compiled_shader) const
{
    dx::ComputeShader shader;
    m_device->CreateComputeShader(compiled_shader.data_ptr(), compiled_shader.data_size(), nullptr, &shader);
    return shader;
}
