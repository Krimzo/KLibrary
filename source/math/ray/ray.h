#pragma once

#include "math/ray/plane.h"
#include "math/ray/sphere.h"
#include "math/triangle/triangle.h"
#include "math/matrix/float4x4.h"


namespace kl {
    struct ray
    {
        float3 origin = {};
        float3 direction = {};

        ray();

        ray(const float3& origin, const float3& direction);

        ray(const float3& origin, const float4x4& inverse_matrix, const float2& ndc);

        bool intersect_plane(const plane& plane, float3* out_intersection) const;

        bool intersect_triangle(const triangle& triangle, float3* out_intersection) const;

        bool intersect_sphere(const sphere& sphere, float3& out_intersection, float* out_distance) const;

        bool intersect_sphere(const sphere& sphere) const;
    };

    std::ostream& operator<<(std::ostream& os, const ray& obj);
}
