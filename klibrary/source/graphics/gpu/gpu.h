#pragma once

#include "graphics/gpu/device_holder.h"
#include "graphics/gpu/context_holder.h"
#include "graphics/text/text_raster.h"
#include "graphics/shaders/shader_compiler.h"


namespace kl {
    enum class GPUCreationType
    {
        NONE = 0,
        RENDER,
        COMPUTE,
    };
}

namespace kl {
    class GPU : public DeviceHolder, public ContextHolder, public ShaderCompiler, public TextRaster
    {
        dx::Chain m_chain;
        dx::Texture m_depth_textures[GPU_BUFFER_COUNT] = {};
        dx::TargetView m_target_views[GPU_BUFFER_COUNT] = {};
        dx::DepthView m_depth_views[GPU_BUFFER_COUNT] = {};

    public:
        const GPUCreationType creation_type = GPUCreationType::NONE;

        // Creation
        GPU(bool debug = false, bool single_threaded = true, bool video_support = false);
        GPU(HWND window, bool debug = false, bool single_threaded = true, bool video_support = false);
        virtual ~GPU();

        // Get
        dx::Device device() const;
        dx::Context context() const;
        dx::Chain chain() const;

        UINT back_index() const;

        dx::Texture target_buffer(UINT index) const;
        dx::Texture depth_buffer(UINT index) const;
        dx::TargetView target_view(UINT index) const;
        dx::DepthView depth_view(UINT index) const;

        dx::Texture back_target_buffer() const;
        dx::Texture back_depth_buffer() const;
        dx::TargetView back_target_view() const;
        dx::DepthView back_depth_view() const;

        // Chain
        void swap_buffers(bool v_sync) const;
        void set_fullscreen(bool enabled) const;
        bool in_fullscreen() const;

        // Internal buffers
        void clear_internal_color(const Float4& color = {}) const;
        void clear_internal_depth(float depth = 1.0f, UINT8 stencil = 0xFF) const;
        void clear_internal(const Float4& color = {}) const;

        void resize_internal(const Int2& size, DXGI_FORMAT depth_format = DXGI_FORMAT_D32_FLOAT);
        void resize_to_window(HWND window);

        void bind_internal_views() const;

        // Shader helper
        ShaderHolder<dx::VertexShader> create_vertex_shader(const std::string& shader_source) const;
        ShaderHolder<dx::GeometryShader> create_geometry_shader(const std::string& shader_source) const;
        ShaderHolder<dx::PixelShader> create_pixel_shader(const std::string& shader_source) const;
        ShaderHolder<dx::ComputeShader> create_compute_shader(const std::string& shader_source) const;

        RenderShaders create_render_shaders(const std::string& shader_sources, const std::vector<dx::LayoutDescriptor>& descriptors = {}) const;

        // Text
        void draw_text() const;
    };
}
