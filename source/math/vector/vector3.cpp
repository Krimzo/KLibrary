#include "math/vector/vector3.h"


// Int3
kl::int3::int3()
{}

kl::int3::int3(const int value)
    : x(value), y(value), z(value)
{}

kl::int3::int3(const int x, const int y, const int z)
    : x(x), y(y), z(z)
{}

// Float3
kl::float3::float3()
{}

kl::float3::float3(const float value)
    : x(value), y(value), z(value)
{}

kl::float3::float3(const float x, const float y, const float z)
    : x(x), y(y), z(z)
{}

kl::float3::float3(const color& color)
{
    x = (color.r * to_float_color);
    y = (color.g * to_float_color);
    z = (color.b * to_float_color);
}

kl::float3::operator kl::color() const
{
    return {
        (byte) min(max((int) (x * to_byte_color), 0), 255),
        (byte) min(max((int) (y * to_byte_color), 0), 255),
        (byte) min(max((int) (z * to_byte_color), 0), 255),
    };
}

kl::float3 kl::float3::operator+(const float3& other) const
{
    return { x + other.x, y + other.y, z + other.z };
}

void kl::float3::operator+=(const float3& other)
{
    *this = (*this + other);
}

kl::float3 kl::float3::operator-(const float3& other) const
{
    return { x - other.x, y - other.y, z - other.z };
}

void kl::float3::operator-=(const float3& other)
{
    *this = (*this - other);
}

kl::float3 kl::float3::operator*(const float value) const
{
    return { x * value, y * value, z * value, };
}

void kl::float3::operator*=(const float value)
{
    *this = (*this * value);
}

float kl::float3::operator*(const float3& other) const
{
    return (x * other.x + y * other.y + z * other.z);
}

bool kl::float3::operator==(const float3& other) const
{
    return (x == other.x && y == other.y && z == other.z);
}

bool kl::float3::operator!=(const float3& other) const
{
    return !(*this == other);
}

float kl::float3::length() const
{
    return sqrt(*this * *this);
}

// Format
std::ostream& kl::operator<<(std::ostream& stream, const int3& vec)
{
    stream << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
    return stream;
}

std::ostream& kl::operator<<(std::ostream& stream, const float3& vec)
{
    stream << std::setprecision(2);
    stream << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
    return stream;
}
