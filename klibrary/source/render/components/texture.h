#pragma once

#include "media/media.h"
#include "graphics/graphics.h"


namespace kl {
    using TextureData = Image;
}

namespace kl {
    class Texture
    {
    public:
        GPU* gpu = nullptr;
        TextureData data_buffer = {};
        dx::Texture graphics_buffer;

        dx::TargetView target_view;
        dx::DepthView depth_view;
        dx::ShaderView shader_view;
        dx::AccessView access_view;

        bool is_cube = false;

        Texture(GPU* gpu);
        Texture(GPU* gpu, const TextureData& data, bool is_cube = false, bool has_unordered_access = false, bool is_target = false);

        void reload(bool has_unordered_access = false, bool is_target = false);
        bool reload_as_cube();

        void create_target_view(const dx::TargetViewDescriptor* descriptor = nullptr);
        void  create_depth_view(const dx::DepthViewDescriptor* descriptor = nullptr);
        void create_shader_view(const dx::ShaderViewDescriptor* descriptor = nullptr);
        void create_access_view(const dx::AccessViewDescriptor* descriptor = nullptr);
    };
}
