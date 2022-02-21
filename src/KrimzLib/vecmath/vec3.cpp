#include "KrimzLib/vecmath/vec3.h"

#include <iostream>

#include "KrimzLib/convert.h"


// Constructors
kl::vec3::vec3() {
	x = 0;
	y = 0;
	z = 0;
}
kl::vec3::vec3(float a) {
	x = a;
	y = a;
	z = a;
}
kl::vec3::vec3(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}
kl::vec3::vec3(const kl::vec2& v, float z) {
	x = v.x;
	y = v.y;
	this->z = z;
}
kl::vec3::vec3(float x, const kl::vec2& v) {
	this->x = x;
	y = v.x;
	z = v.y;
}
kl::vec3::vec3(const kl::color& color) {
	static const float toFloatCol = 1.0f / 255;
	x = color.r * toFloatCol;
	y = color.g * toFloatCol;
	z = color.b * toFloatCol;
}

// Addition
kl::vec3 kl::vec3::add(const kl::vec3& obj) const {
	return kl::vec3(x + obj.x, y + obj.y, z + obj.z);
}
kl::vec3 kl::vec3::operator+(const kl::vec3& obj) const {
	return this->add(obj);
}
void kl::vec3::operator+=(const kl::vec3& obj) {
	x += obj.x; y += obj.y; z += obj.z;
}

// Subtraction
kl::vec3 kl::vec3::sub(const kl::vec3& obj) const {
	return kl::vec3(x - obj.x, y - obj.y, z - obj.z);
}
kl::vec3 kl::vec3::operator-(const kl::vec3& obj) const {
	return this->sub(obj);
}
void kl::vec3::operator-=(const kl::vec3& obj) {
	x -= obj.x; y -= obj.y; z -= obj.z;
}

// Multiplication
kl::vec3 kl::vec3::mul(float a) const {
	return kl::vec3(x * a, y * a, z * a);
}
kl::vec3 kl::vec3::operator*(float a) const {
	return this->mul(a);
}
void kl::vec3::operator*=(float a) {
	x *= a; y *= a; z *= a;
}
kl::vec3 kl::vec3::mul(const kl::vec3& obj) const {
	return kl::vec3(x * obj.x, y * obj.y, z * obj.z);
}
kl::vec3 kl::vec3::operator*(const kl::vec3& obj) const {
	return this->mul(obj);
}
void kl::vec3::operator*=(const kl::vec3& obj) {
	x *= obj.x; y *= obj.y; z *= obj.z;
}

// Division
kl::vec3 kl::vec3::div(float a) const {
	return this->mul(1 / a);
}
kl::vec3 kl::vec3::operator/(float a) const {
	return this->div(a);
}
void kl::vec3::operator/=(float a) {
	this->operator*=(1 / a);
}
kl::vec3 kl::vec3::div(const kl::vec3& obj) const {
	return kl::vec3(x / obj.x, y / obj.y, z / obj.z);
}
kl::vec3 kl::vec3::operator/(const kl::vec3& obj) const {
	return this->div(obj);
}
void kl::vec3::operator/=(const kl::vec3& obj) {
	x /= obj.x; y /= obj.y; z /= obj.z;
}

// Comparison
bool kl::vec3::equals(const kl::vec3& obj) const {
	return x == obj.x && y == obj.y && z == obj.z;
}
bool kl::vec3::operator==(const kl::vec3& obj) const {
	return this->equals(obj);
}
bool kl::vec3::operator!=(const kl::vec3& obj) const {
	return !this->equals(obj);
}

// Returns a negated vector
kl::vec3 kl::vec3::negate() const {
	return this->mul(-1);
}

// Returns an absolute vector
kl::vec3 kl::vec3::abso() const {
	return kl::vec3(abs(x), abs(y), abs(z));
}

// Returns the vectors length
float kl::vec3::length() const {
	return sqrt(x * x + y * y + z * z);
}

// Retruns a normalized vector
kl::vec3 kl::vec3::normalize() const {
	return this->div(this->length());
}

// Returns the dot product
float kl::vec3::dot(const kl::vec3& a) const {
	return x * a.x + y * a.y + z * a.z;
}

// Returns the dot product
kl::vec3 kl::vec3::cross(const kl::vec3& a) const {
	return kl::vec3(y * a.z - z * a.y, z * a.x - x * a.z, x * a.y - y * a.x);
}

// Returns the angle between the given vector and self
float kl::vec3::angle(const kl::vec3& a) const {
	return kl::convert::toDegrees(acos(this->normalize().dot(a.normalize())));
}

// Returns a rotated vector around the given axis
kl::vec3 kl::vec3::rotate(float angle, const kl::vec3& axis) const {
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
	kl::vec3 temp;
	temp.x = (w2 + x2 - z2 - y2) * x + (-zw + xy - zw + xy) * y + (yw + xz + xz + yw) * z;
	temp.y = (xy + zw + zw + xy) * x + (y2 - z2 + w2 - x2) * y + (yz + yz - xw - xw) * z;
	temp.z = (xz - yw + xz - yw) * x + (yz + yz + xw + xw) * y + (z2 - y2 - x2 + w2) * z;
	return temp;
}

// Overloading std::cout
std::ostream& kl::operator<<(std::ostream& os, const kl::vec3& obj) {
	os << kl::convert::toString(obj.x) << " " << kl::convert::toString(obj.y) << " " << kl::convert::toString(obj.z);
	return os;
}
