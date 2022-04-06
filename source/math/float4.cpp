#include "math/float4.h"

#include <iostream>

#include "utility/convert.h"


// Constructors
kl::float4::float4()
{
	x = 0;
	y = 0;
	z = 0;
	w = 0;
}
kl::float4::float4(float a)
{
	x = a;
	y = a;
	z = a;
	w = a;
}
kl::float4::float4(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}
kl::float4::float4(const kl::int4& v)
{
	x = float(v.x);
	y = float(v.y);
	z = float(v.z);
	w = float(v.w);
}
kl::float4::float4(const kl::float2& v, float z, float w)
{
	x = v.x;
	y = v.y;
	this->z = z;
	this->w = w;
}
kl::float4::float4(float x, const kl::float2& v, float w)
{
	this->x = x;
	y = v.x;
	z = v.y;
	this->w = w;
}
kl::float4::float4(float x, float y, const kl::float2& v)
{
	this->x = x;
	this->y = y;
	z = v.x;
	w = v.y;
}
kl::float4::float4(const kl::float2& v1, const kl::float2& v2)
{
	x = v1.x;
	y = v1.y;
	z = v2.x;
	w = v2.y;
}
kl::float4::float4(const kl::float3& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	w = 1;
}
kl::float4::float4(const kl::float3& v, float w)
{
	x = v.x;
	y = v.y;
	z = v.z;
	this->w = w;
}
kl::float4::float4(float x, const kl::float3& v)
{
	this->x = x;
	y = v.x;
	z = v.y;
	w = v.z;
}
kl::float4::float4(const kl::color& color)
{
	static const float toFloatCol = 1.0f / 255;
	x = color.r * toFloatCol;
	y = color.g * toFloatCol;
	z = color.b * toFloatCol;
	w = color.a * toFloatCol;
}

// Getter
kl::float3 kl::float4::xyz() const
{
	return kl::float3(x, y, z);
}

// Addition
kl::float4 kl::float4::add(const kl::float4& obj) const
{
	return kl::float4(x + obj.x, y + obj.y, z + obj.z, w + obj.w);
}
kl::float4 kl::float4::operator+(const kl::float4& obj) const
{
	return this->add(obj);
}
void kl::float4::operator+=(const kl::float4& obj)
{
	x += obj.x; y += obj.y; z += obj.z; w += obj.w;
}

// Subtraction
kl::float4 kl::float4::sub(const kl::float4& obj) const
{
	return kl::float4(x - obj.x, y - obj.y, z - obj.z, w - obj.w);
}
kl::float4 kl::float4::operator-(const kl::float4& obj) const
{
	return this->sub(obj);
}
void kl::float4::operator-=(const kl::float4& obj)
{
	x -= obj.x; y -= obj.y; z -= obj.z; w -= obj.w;
}

// Multiplication
kl::float4 kl::float4::mul(float a) const
{
	return kl::float4(x * a, y * a, z * a, w * a);
}
kl::float4 kl::float4::operator*(float a) const
{
	return this->mul(a);
}
void kl::float4::operator*=(float a)
{
	x *= a; y *= a; z *= a; w *= a;
}
kl::float4 kl::float4::mul(const kl::float4& obj) const
{
	return kl::float4(x * obj.x, y * obj.y, z * obj.z, w * obj.w);
}
kl::float4 kl::float4::operator*(const kl::float4& obj) const
{
	return this->mul(obj);
}
void kl::float4::operator*=(const kl::float4& obj)
{
	x *= obj.x; y *= obj.y; z *= obj.z; w *= obj.w;
}

// Division
kl::float4 kl::float4::div(float a) const
{
	return this->mul(1 / a);
}
kl::float4 kl::float4::operator/(float a) const
{
	return this->div(a);
}
void kl::float4::operator/=(float a)
{
	this->operator*=(1 / a);
}
kl::float4 kl::float4::div(const kl::float4& obj) const
{
	return kl::float4(x / obj.x, y / obj.y, z / obj.z, w / obj.w);
}
kl::float4 kl::float4::operator/(const kl::float4& obj) const
{
	return this->div(obj);
}
void kl::float4::operator/=(const kl::float4& obj)
{
	x /= obj.x; y /= obj.y; z /= obj.z; w /= obj.w;
}

// Comparison
bool kl::float4::equals(const kl::float4& obj) const
{
	return x == obj.x && y == obj.y && z == obj.z && w == obj.w;
}
bool kl::float4::operator==(const kl::float4& obj) const
{
	return this->equals(obj);
}
bool kl::float4::operator!=(const kl::float4& obj) const
{
	return !this->equals(obj);
}

// Returns a negated vector
kl::float4 kl::float4::negate() const
{
	return this->mul(-1);
}

// Returns an absolute vector
kl::float4 kl::float4::abso() const
{
	return kl::float4(abs(x), abs(y), abs(z), abs(w));
}

// Returns the vectors length
float kl::float4::length() const
{
	return sqrt(x * x + y * y + z * z + w * w);
}

// Retruns a normalized vector
kl::float4 kl::float4::normalize() const
{
	return this->div(this->length());
}

// Returns the dot product
float kl::float4::dot(const kl::float4& a) const
{
	return x * a.x + y * a.y + z * a.z + w * a.w;
}

// Returns the angle between the given vector and self
float kl::float4::angle(const kl::float4& a) const
{
	return kl::convert::toDegrees(acos(this->normalize().dot(a.normalize())));
}

// Overloading std::cout
std::ostream& kl::operator<<(std::ostream& os, const kl::float4& obj)
{
	os << "(" << kl::convert::toString(obj.x) << ", " << kl::convert::toString(obj.y) << ", " << kl::convert::toString(obj.z) << ", " << kl::convert::toString(obj.w) << ")";
	return os;
}
