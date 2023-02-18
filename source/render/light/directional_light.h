#pragma once

#include "render/scene/camera.h"
#include "graphics/graphics.h"


namespace kl {
    class directional_light
    {
    public:
        static constexpr UINT MAP_COUNT = 4;
        static constexpr float CASCADE_SPLITS[MAP_COUNT + 1] = { 0.0f, 0.075f, 0.2f, 0.5f, 1.0f };

    private:
        float3 direction_ = { 0, 0, 1 };
        UINT map_resolution_ = 0;

        ref<gpu_depth_view>   shadow_depth_views_[MAP_COUNT] = {};
        ref<gpu_shader_view> shadow_shader_views_[MAP_COUNT] = {};

        directional_light(UINT map_resolution);

    public:
        float point_size = 1.0f;

        static ref<directional_light> make(UINT map_resolution);

        virtual ~directional_light();

        directional_light(const directional_light&) = delete;
        directional_light(const directional_light&&) = delete;

        void operator=(const directional_light&) = delete;
        void operator=(const directional_light&&) = delete;

        void set_direction(const float3& direction);
        float3 get_direction() const;

        UINT get_map_resolution() const;

        dx::depth_view   get_depth_view(UINT cascade_index) const;
        dx::shader_view get_shader_view(UINT cascade_index) const;

        float4x4 get_matrix(camera camera, UINT cascade_index) const;
    };
}
