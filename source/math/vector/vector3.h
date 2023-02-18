#pragma once

#include "math/vector/vector2.h"
#include "media/image/color.h"


namespace kl {
    inline constexpr float to_float_color = 1.0f / 255.0f;
    inline constexpr float to_byte_color = 255.0f;
}

namespace kl {
    struct int3
    {
        int x = 0;
        int y = 0;
        int z = 0;

        int3();
        explicit int3(int value);
        int3(int x, int y, int z);
    };
}

namespace kl {
    struct float3
    {
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;

        float3();
        explicit float3(float value);
        float3(float x, float y, float z);

        float3(const color& color);
        operator color() const;

        float3 operator+(const float3& other) const;
        void operator+=(const float3& other);

        float3 operator-(const float3& other) const;
        void operator-=(const float3& other);

        float3 operator*(float value) const;
        void operator*=(float value);

        float operator*(const float3& other) const;

        bool operator==(const float3& other) const;
        bool operator!=(const float3& other) const;

        float length() const;
    };
}

namespace kl {
    std::ostream& operator<<(std::ostream& stream, const int3& vec);
    std::ostream& operator<<(std::ostream& stream, const float3& vec);
}
