#pragma once

#include "math/vector/float4.h"


namespace kl {
    struct plane
    {
        float3 origin = {};
        float3 normal = {};

        plane();
        plane(const float3& origin, const float3& normal);

        bool is_in_front(const float3& point) const;
    };
}

namespace kl {
    std::ostream& operator<<(std::ostream& os, const plane& obj);
}
