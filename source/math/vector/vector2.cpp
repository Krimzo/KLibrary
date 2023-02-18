#include "math/vector/vector2.h"


// Int2
kl::int2::int2()
{}

kl::int2::int2(const int value)
    : x(value), y(value)
{}

kl::int2::int2(const int x, const int y)
    : x(x), y(y)
{}

// Float2
kl::float2::float2()
{}

kl::float2::float2(const float value)
    : x(value), y(value)
{}

kl::float2::float2(const float x, const float y)
    : x(x), y(y)
{}

kl::float2 kl::float2::operator+(const float2& other) const
{
    return { x + other.x, y + other.y };
}

void kl::float2::operator+=(const float2& other)
{
    *this = (*this + other);
}

kl::float2 kl::float2::operator-(const float2& other) const
{
    return { x - other.x, y - other.y };
}

void kl::float2::operator-=(const float2& other)
{
    *this = (*this - other);
}

kl::float2 kl::float2::operator*(const float value) const
{
    return { x * value, y * value };
}

void kl::float2::operator*=(const float value)
{
    *this = (*this * value);
}

float kl::float2::operator*(const float2& other) const
{
    return (x * other.x + y * other.y);
}

bool kl::float2::operator==(const float2& other) const
{
    return (x == other.x && y == other.y);
}

bool kl::float2::operator!=(const float2& other) const
{
    return !(*this == other);
}

float kl::float2::length() const
{
    return sqrt(*this * *this);
}

// Format
std::ostream& kl::operator<<(std::ostream& stream, const int2& vec)
{
    stream << "(" << vec.x << ", " << vec.y << ")";
    return stream;
}

std::ostream& kl::operator<<(std::ostream& stream, const float2& vec)
{
    stream << std::setprecision(2);
    stream << "(" << vec.x << ", " << vec.y << ")";
    return stream;
}
