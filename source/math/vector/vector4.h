#pragma once

#include "math/vector/vector3.h"


namespace kl {
    struct int4
    {
        int x = 0;
        int y = 0;
        int z = 0;
        int w = 0;

        int4();
        explicit int4(int value);
        int4(int x, int y, int z, int w);
    };
}

namespace kl {
    struct float4
    {
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
        float w = 0.0f;

        float4();
        explicit float4(float value);
        float4(float x, float y, float z, float w);

        float4(const color& color);
        operator color() const;

        float4 operator+(const float4& other) const;
        void operator+=(const float4& other);

        float4 operator-(const float4& other) const;
        void operator-=(const float4& other);

        float4 operator*(float value) const;
        void operator*=(float value);

        float operator*(const float4& other) const;

        bool operator==(const float4& other) const;
        bool operator!=(const float4& other) const;

        float length() const;
    };
}

namespace kl {
    std::ostream& operator<<(std::ostream& stream, const int4& vec);
    std::ostream& operator<<(std::ostream& stream, const float4& vec);
}
