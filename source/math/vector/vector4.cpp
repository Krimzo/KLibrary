#include "math/vector/vector4.h"


// Int4
kl::int4::int4()
{}

kl::int4::int4(const int value)
    : x(value), y(value), z(value)
{}

kl::int4::int4(const int x, const int y, const int z, const int w)
    : x(x), y(y), z(z), w(w)
{}

// Float4
kl::float4::float4()
{}

kl::float4::float4(const float value)
    : x(value), y(value), z(value), w(value)
{}

kl::float4::float4(const float x, const float y, const float z, const float w)
    : x(x), y(y), z(z), w(w)
{}

kl::float4::float4(const color& color)
{
    x = (color.r * to_float_color);
    y = (color.g * to_float_color);
    z = (color.b * to_float_color);
    w = (color.a * to_float_color);
}

kl::float4::operator kl::color() const
{
    return {
        (byte) min(max((int) (x * to_byte_color), 0), 255),
        (byte) min(max((int) (y * to_byte_color), 0), 255),
        (byte) min(max((int) (z * to_byte_color), 0), 255),
        (byte) min(max((int) (w * to_byte_color), 0), 255),
    };
}

kl::float4 kl::float4::operator+(const float4& other) const
{
    return { x + other.x, y + other.y, z + other.z, w + other.w };
}

void kl::float4::operator+=(const float4& other)
{
    *this = (*this + other);
}

kl::float4 kl::float4::operator-(const float4& other) const
{
    return { x - other.x, y - other.y, z - other.z, w - other.w };
}

void kl::float4::operator-=(const float4& other)
{
    *this = (*this - other);
}

kl::float4 kl::float4::operator*(const float value) const
{
    return { x * value, y * value, z * value, w * value };
}

void kl::float4::operator*=(const float value)
{
    *this = (*this * value);
}

float kl::float4::operator*(const float4& other) const
{
    return (x * other.x + y * other.y + z * other.z + w * other.w);
}

bool kl::float4::operator==(const float4& other) const
{
    return (x == other.x && y == other.y && z == other.z && w == other.w);
}

bool kl::float4::operator!=(const float4& other) const
{
    return !(*this == other);
}

float kl::float4::length() const
{
    return sqrt(*this * *this);
}

// Format
std::ostream& kl::operator<<(std::ostream& stream, const int4& vec)
{
    stream << "(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
    return stream;
}

std::ostream& kl::operator<<(std::ostream& stream, const float4& vec)
{
    stream << std::setprecision(2);
    stream << "(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
    return stream;
}
