#pragma once

#include "math/triangle/vertex.h"


namespace kl {
    struct triangle
    {
        vertex a = {};
        vertex b = {};
        vertex c = {};

        triangle();

        triangle(const vertex& a, const vertex& b, const vertex& c);

        float4 get_constants() const;

        float3 get_weights(const float4& constants, const float2& point) const;

        float3 get_weights(const float3& position) const;

        bool is_in_triangle(const float3& weights);

        float interpolate(const float3& weights, const float3& values) const;

        vertex interpolate(const float3& weights) const;
    };

    std::ostream& operator<<(std::ostream& os, const triangle& obj);
}
