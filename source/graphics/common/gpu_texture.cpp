#include "graphics/common/gpu_texture.h"

#include "utility/utility.h"


// Makers
kl::ref<kl::gpu_texture> kl::gpu_texture::make(const dx::texture_descriptor* descriptor, const dx::subresource_descriptor* subresource_data)
{
    return ref<gpu_texture>(new gpu_texture(descriptor, subresource_data));
}

kl::ref<kl::gpu_texture> kl::gpu_texture::make(const image& image, const bool has_unordered_access, const bool is_target)
{
    return ref<gpu_texture>(new gpu_texture(image, has_unordered_access, is_target));
}

kl::ref<kl::gpu_texture> kl::gpu_texture::make(const image& front, const image& back, const image& left, const image& right, const image& top, const image& bottom)
{
    return ref<gpu_texture>(new gpu_texture(front, back, left, right, top, bottom));
}

kl::ref<kl::gpu_texture> kl::gpu_texture::make(const dx::texture texture, const int2& size)
{
    return ref<gpu_texture>(new gpu_texture(texture, size));
}

// Class
kl::gpu_texture::gpu_texture()
{}

kl::gpu_texture::gpu_texture(const dx::texture_descriptor* descriptor, const dx::subresource_descriptor* subresource_data)
    : creation_type(gpu_texture_creation_type::normal)
{
    const long result = BOUND_GPU->device()->CreateTexture2D(descriptor, subresource_data, &child_object_);
    warning_check(!child_object_, format("Failed to create texture. Result: 0x", std::hex, result));
}

kl::gpu_texture::gpu_texture(const image& image, const bool has_unordered_access, const bool is_target)
    : creation_type(gpu_texture_creation_type::normal)
{
    const kl::image flipped_image = image.flip_vertical();

    dx::texture_descriptor descriptor = {};
    descriptor.Width = image.width();
    descriptor.Height = image.height();
    descriptor.MipLevels = 1;
    descriptor.ArraySize = 1;
    descriptor.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    descriptor.SampleDesc.Count = 1;
    descriptor.Usage = D3D11_USAGE_DEFAULT;
    descriptor.BindFlags = D3D11_BIND_SHADER_RESOURCE |
        (has_unordered_access ? D3D11_BIND_UNORDERED_ACCESS : NULL) |
        (is_target ? D3D11_BIND_RENDER_TARGET : NULL);

    dx::subresource_descriptor data = {};
    data.pSysMem = flipped_image;
    data.SysMemPitch = image.width() * sizeof(color);

    this->gpu_texture::gpu_texture(&descriptor, &data);
}

kl::gpu_texture::gpu_texture(const image& front, const image& back, const image& left, const image& right, const image& top, const image& bottom)
    : creation_type(gpu_texture_creation_type::cube)
{
    if (warning_check(
        front.width() != back.width() || front.height() != back.height() ||
        front.width() != left.width() || front.height() != left.height() ||
        front.width() != right.width() || front.height() != right.height() ||
        front.width() != top.width() || front.height() != top.height() ||
        front.width() != bottom.width() || front.height() != bottom.height(), "Sizes of the 6 given images do not match")) {
        return;
    }

    dx::texture_descriptor descriptor = {};
    descriptor.Width = front.width();
    descriptor.Height = front.height();
    descriptor.MipLevels = 1;
    descriptor.ArraySize = 6;
    descriptor.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    descriptor.SampleDesc.Count = 1;
    descriptor.Usage = D3D11_USAGE_DEFAULT;
    descriptor.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    descriptor.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

    const UINT mem_pitch = UINT(front.width() * sizeof(color));
    const dx::subresource_descriptor data[6] = {
        { right,  mem_pitch, 0 },
        { left,   mem_pitch, 0 },
        { top,    mem_pitch, 0 },
        { bottom, mem_pitch, 0 },
        { front,  mem_pitch, 0 },
        { back,   mem_pitch, 0 },
    };

    this->gpu_texture::gpu_texture(&descriptor, data);
}

kl::gpu_texture::gpu_texture(const dx::texture texture, const int2& size)
    : creation_type(gpu_texture_creation_type::staging)
{
    dx::texture_descriptor descriptor = {};
    texture->GetDesc(&descriptor);

    dx::texture_descriptor staging_descriptor = {};
    staging_descriptor.Width = (size.x > 0) ? size.x : descriptor.Width;
    staging_descriptor.Height = (size.y > 0) ? size.y : descriptor.Height;
    staging_descriptor.MipLevels = 1;
    staging_descriptor.ArraySize = 1;
    staging_descriptor.Format = descriptor.Format;
    staging_descriptor.SampleDesc.Count = 1;
    staging_descriptor.Usage = D3D11_USAGE_STAGING;
    staging_descriptor.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

    this->gpu_texture::gpu_texture(&staging_descriptor, nullptr);
}

// Static
kl::ref<kl::gpu_texture> kl::gpu_texture::get_back_buffer(dx::chain chain)
{
    dx::texture buffer = nullptr;
    const long result = chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**) &buffer);
    warning_check(!buffer, format("Failed to get backbuffer texture. Result: 0x", std::hex, result));

    auto texture = ref<gpu_texture>(new gpu_texture());
    texture->child_object_ = buffer;
    return texture;
}
