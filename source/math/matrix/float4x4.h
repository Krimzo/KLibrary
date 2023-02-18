#pragma once

#include "math/vector/vector4.h"


namespace kl {
    struct float4x4
    {
        float data[16] = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f,
        };

        float4x4();

        float& operator[](int index);
        const float& operator[](int index) const;

        float& operator()(int x, int y);
        const float& operator()(int x, int y) const;

        float4x4 operator+(const float4x4& other) const;
        void operator+=(const float4x4& other);

        float4x4 operator-(const float4x4& other) const;
        void operator-=(const float4x4& other);

        float4x4 operator*(float value) const;
        void operator*=(float value);

        float4x4 operator*(const float4x4& other) const;
        void operator*=(const float4x4& other);

        float4 operator*(const float4& vec) const;

        bool operator==(const float4x4& other) const;
        bool operator!=(const float4x4& other) const;

        float determinant() const;

        static float4x4 translation(const float3& translation);
        static float4x4 rotation(const float3& rotation);
        static float4x4 scaling(const float3& scale);

        static float4x4 perspective(float field_of_view, float aspect_ratio, float near_plane, float far_plane);
        static float4x4 orthographic(float left, float right, float bottom, float top, float near_plane, float far_plane);

        static float4x4 look_at(const float3& position, const float3& target, const float3& up);
    };

    std::ostream& operator<<(std::ostream& stream, const float4x4& mat);
}
