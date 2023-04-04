#include "graphics/shaders/shader_holder.h"

#include "graphics/gpu/gpu.h"


// Construct
template<typename T>
kl::shader_holder<T>::shader_holder(kl::gpu* gpu)
    : gpu(gpu)
{}

template<typename T>
kl::shader_holder<T>::shader_holder(kl::gpu* gpu, const T& shader)
    : gpu(gpu), shader(shader)
{}

template<typename T>
kl::shader_holder<T>::~shader_holder()
{}

// Get
template<typename T>
kl::shader_holder<T>::operator T() const
{
    return shader;
}

template<typename T>
kl::shader_holder<T>::operator bool() const
{
    return (bool) shader;
}

// Update
template<typename T>
void kl::shader_holder<T>::update_cbuffer(const void* data, UINT byte_size)
{
    // Fix size scaling
    const UINT size_mod = byte_size % 16;
    if (size_mod) {
        byte_size += (16 - size_mod);
    }

    // Regenerate buffer if needed
    const UINT buffer_size = cbuffer ? gpu->get_buffer_size(cbuffer) : 0;
    if (byte_size != buffer_size) {
        cbuffer = gpu->create_const_buffer(byte_size);
    }

    // Write data
    gpu->write_to_resource(cbuffer, data, byte_size);

    // Rebind cbuffer
    if constexpr (std::is_same<T, dx::vertex_shader>::value) {
        gpu->bind_cb_for_vertex_shader(cbuffer, 0);
    }
    else if constexpr (std::is_same<T, dx::geometry_shader>::value) {
        gpu->bind_cb_for_geometry_shader(cbuffer, 0);
    }
    else if constexpr (std::is_same<T, dx::pixel_shader>::value) {
        gpu->bind_cb_for_pixel_shader(cbuffer, 0);
    }
    else if constexpr (std::is_same<T, dx::compute_shader>::value) {
        gpu->bind_cb_for_compute_shader(cbuffer, 0);
    }
}

// Allowed types
template struct kl::shader_holder<kl::dx::vertex_shader>;
template struct kl::shader_holder<kl::dx::geometry_shader>;
template struct kl::shader_holder<kl::dx::pixel_shader>;
template struct kl::shader_holder<kl::dx::compute_shader>;
