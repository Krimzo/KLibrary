#pragma once

#include "apis/apis.h"


namespace kl::math {
    inline constexpr float pi = 3.14159265358979f;
    inline constexpr float to_radians = pi / 180.0f;
    inline constexpr float to_degrees = 180.0f / pi;
}

namespace kl {
    struct int2
    {
        int x = 0;
        int y = 0;

        int2();
        explicit int2(int value);
        int2(int x, int y);
    };
}

namespace kl {
    struct float2
    {
        float x = 0.0f;
        float y = 0.0f;

        float2();
        explicit float2(float value);
        float2(float x, float y);

        float2 operator+(const float2& other) const;
        void operator+=(const float2& other);

        float2 operator-(const float2& other) const;
        void operator-=(const float2& other);

        float2 operator*(float value) const;
        void operator*=(float value);

        float operator*(const float2& other) const;

        bool operator==(const float2& other) const;
        bool operator!=(const float2& other) const;

        float length() const;
    };
}

namespace kl {
    std::ostream& operator<<(std::ostream& stream, const int2& vec);
    std::ostream& operator<<(std::ostream& stream, const float2& vec);
}
