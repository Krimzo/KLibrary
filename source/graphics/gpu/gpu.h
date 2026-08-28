#pragma once

#include "graphics/gpu/device_holder.h"
#include "graphics/gpu/context_holder.h"
#include "graphics/raster/raster.h"
#include "graphics/shaders/shader_compiler.h"

namespace kl
{
struct GPU : DeviceHolder, ContextHolder, ShaderCompiler, Raster
{
    GPU(HWND window, bool debug = IS_DEBUG, bool has_unordered_access = false, bool video_support = false);
    virtual ~GPU() noexcept;

    dx::Device device() const;
    dx::Context context() const;
    dx::Chain chain() const;

    UINT back_index() const;

    dx::Texture target_texture(UINT index) const;
    dx::Texture depth_texture(UINT index) const;
    dx::TargetView target_view(UINT index) const;
    dx::DepthView depth_view(UINT index) const;

    dx::Texture back_target_texture() const;
    dx::Texture back_depth_texture() const;
    dx::TargetView back_target_view() const;
    dx::DepthView back_depth_view() const;

    void swap_buffers(bool v_sync) const;

    bool fullscreened() const;
    void set_fullscreen(bool enabled) const;

    void clear_internal_color(float4 color = {}) const;
    void clear_internal_depth(float depth = 1.0f, UINT8 stencil = 0xFF) const;
    void clear_internal(float4 color = {}, float depth = 1.0f, UINT8 stencil = 0xFF) const;

    void resize_internal(int2 size, DXGI_FORMAT depth_format = DXGI_FORMAT_D32_FLOAT);
    void resize_to_window(HWND window);

    void bind_internal_views() const;

    VertexShader create_vertex_shader(std::string_view shader_source, std::string* out_error = nullptr) const;
    PixelShader create_pixel_shader(std::string_view shader_source, std::string* out_error = nullptr) const;
    GeometryShader create_geometry_shader(std::string_view shader_source, std::string* out_error = nullptr) const;
    ComputeShader create_compute_shader(std::string_view shader_source, std::string* out_error = nullptr) const;
    Shaders create_shaders(std::string_view shader_sources,
                           std::initializer_list<dx::LayoutDescriptor> const& descriptors = {},
                           std::string* out_vs_error = nullptr, std::string* out_ps_error = nullptr) const;

    void draw_raster_batch() const;
    void draw_raster_direct(RasterItem const& raster_item) const;

  private:
    dx::Chain m_chain;
    dx::Texture m_depth_textures[GPU_BUFFER_COUNT];
    dx::TargetView m_target_views[GPU_BUFFER_COUNT];
    dx::DepthView m_depth_views[GPU_BUFFER_COUNT];
};
} // namespace kl

namespace kl
{
struct VideoGPU : GPU
{
    inline VideoGPU() : GPU(nullptr, IS_DEBUG, false, true)
    {
    }
};
} // namespace kl
