#pragma once

#include "graphics/graphics.h"


namespace kl {
    class material
    {
    public:
        float4 color = {};

        ref<gpu_shader_view>     color_map = nullptr;
        ref<gpu_shader_view>    normal_map = nullptr;
        ref<gpu_shader_view> roughness_map = nullptr;

        float texture_blend = 0.0f;
        float reflection_factor = 0.0f;
        float refraction_factor = 0.0f;
        float refraction_index = 0.0f;

        material();
        ~material();
    };
}
