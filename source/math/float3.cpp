#include "math/float3.h"

#include <iostream>

#include "utility/convert.h"


// Constr
kl::float3::float3() : x(0.0f), y(0.0f), z(0.0f) {}
kl::float3::float3(float a) : x(a), y(a), z(a) {}
kl::float3::float3(float x, float y, float z) : x(x), y(y), z(z) {}
kl::float3::float3(const kl::int3& v) : x(float(v.x)), y(float(v.y)), z(float(v.z)) {}
kl::float3::float3(const kl::float2& v, float z) : x(v.x), y(v.y), z(z) {}
kl::float3::float3(float x, const kl::float2& v) : x(x), y(v.x), z(v.y) {}
kl::float3::float3(const kl::color& c) : x(kl::convert::toFloCol(c.r)), y(kl::convert::toFloCol(c.g)), z(kl::convert::toFloCol(c.b)) {}

// Getters
kl::float2 kl::float3::xy() const {
	return kl::float2(x, y);
}
float& kl::float3::operator[](int i) {
	return data[i];
}
const float& kl::float3::operator[](int i) const {
	return data[i];
}

// Addition
kl::float3 kl::float3::add(const kl::float3& obj) const {
	return kl::float3(x + obj.x, y + obj.y, z + obj.z);
}
kl::float3 kl::float3::operator+(const kl::float3& obj) const {
	return add(obj);
}
void kl::float3::operator+=(const kl::float3& obj) {
	x += obj.x; y += obj.y; z += obj.z;
}

// Subtraction
kl::float3 kl::float3::sub(const kl::float3& obj) const {
	return kl::float3(x - obj.x, y - obj.y, z - obj.z);
}
kl::float3 kl::float3::operator-(const kl::float3& obj) const {
	return sub(obj);
}
void kl::float3::operator-=(const kl::float3& obj) {
	x -= obj.x; y -= obj.y; z -= obj.z;
}

// Multiplication
kl::float3 kl::float3::mul(float a) const {
	return kl::float3(x * a, y * a, z * a);
}
kl::float3 kl::float3::operator*(float a) const {
	return mul(a);
}
void kl::float3::operator*=(float a) {
	x *= a; y *= a; z *= a;
}
kl::float3 kl::float3::mul(const kl::float3& obj) const {
	return kl::float3(x * obj.x, y * obj.y, z * obj.z);
}
kl::float3 kl::float3::operator*(const kl::float3& obj) const {
	return mul(obj);
}
void kl::float3::operator*=(const kl::float3& obj) {
	x *= obj.x; y *= obj.y; z *= obj.z;
}

// Division
kl::float3 kl::float3::div(float a) const {
	return mul(1.0f / a);
}
kl::float3 kl::float3::operator/(float a) const {
	return div(a);
}
void kl::float3::operator/=(float a) {
	operator*=(1.0f / a);
}
kl::float3 kl::float3::div(const kl::float3& obj) const {
	return kl::float3(x / obj.x, y / obj.y, z / obj.z);
}
kl::float3 kl::float3::operator/(const kl::float3& obj) const {
	return div(obj);
}
void kl::float3::operator/=(const kl::float3& obj) {
	x /= obj.x; y /= obj.y; z /= obj.z;
}

// Comparison
bool kl::float3::equals(const kl::float3& obj) const {
	return x == obj.x && y == obj.y && z == obj.z;
}
bool kl::float3::operator==(const kl::float3& obj) const {
	return equals(obj);
}
bool kl::float3::operator!=(const kl::float3& obj) const {
	return !equals(obj);
}

// Returns a negated vector
kl::float3 kl::float3::negate() const {
	return mul(-1.0f);
}

// Returns an absolute vector
kl::float3 kl::float3::abso() const {
	return kl::float3(abs(x), abs(y), abs(z));
}

// Returns the vectors length
float kl::float3::length() const {
	return sqrt(x * x + y * y + z * z);
}

// Retruns a normalized vector
kl::float3 kl::float3::normalize() const {
	return div(length());
}

// Returns the dot product
float kl::float3::dot(const kl::float3& a) const {
	return x * a.x + y * a.y + z * a.z;
}

// Returns the cross product
kl::float3 kl::float3::cross(const kl::float3& a) const {
	return kl::float3(y * a.z - z * a.y, z * a.x - x * a.z, x * a.y - y * a.x);
}

// Returns the angle between the given vector and self
float kl::float3::angle(const kl::float3& a) const {
	return kl::convert::toDegrees(acos(normalize().dot(a.normalize())));
}

// Returns a rotated vector around the given axis
kl::float3 kl::float3::rotate(float angle, const kl::float3& axis) const {
	// Calculating trig funcs
	const float angleSin = sin(kl::convert::toRadians(angle) * 0.5f);
	const float angleCos = cos(kl::convert::toRadians(angle) * 0.5f);

	// Calculating quaternion consts
	const float qx = axis.x * angleSin;
	const float qy = axis.y * angleSin;
	const float qz = axis.z * angleSin;
	const float x2 = qx * qx;
	const float y2 = qy * qy;
	const float z2 = qz * qz;
	const float w2 = angleCos * angleCos;
	const float xy = qx * qy;
	const float xz = qx * qz;
	const float yz = qy * qz;
	const float xw = qx * angleCos;
	const float yw = qy * angleCos;
	const float zw = qz * angleCos;

	// Calculating the rotated vector
	kl::float3 temp;
	temp.x = (w2 + x2 - z2 - y2) * x + (-zw + xy - zw + xy) * y + (yw + xz + xz + yw) * z;
	temp.y = (xy + zw + zw + xy) * x + (y2 - z2 + w2 - x2) * y + (yz + yz - xw - xw) * z;
	temp.z = (xz - yw + xz - yw) * x + (yz + yz + xw + xw) * y + (z2 - y2 - x2 + w2) * z;
	return temp;
}

// Constants
const kl::float3 kl::float3::pos_x = kl::float3(1.0f, 0.0f, 0.0f);
const kl::float3 kl::float3::neg_x = kl::float3(-1.0f, 0.0f, 0.0f);
const kl::float3 kl::float3::pos_y = kl::float3(0.0f, 1.0f, 0.0f);
const kl::float3 kl::float3::neg_y = kl::float3(0.0f, -1.0f, 0.0f);
const kl::float3 kl::float3::pos_z = kl::float3(0.0f, 0.0f, 1.0f);
const kl::float3 kl::float3::neg_z = kl::float3(0.0f, 0.0f, -1.0f);

// Overloading std::cout
std::ostream& kl::operator<<(std::ostream& os, const kl::float3& obj) {
	os << "(" << obj.x << ", " << obj.y << ", " << obj.z << ")";
	return os;
}
