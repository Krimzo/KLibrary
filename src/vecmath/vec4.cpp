#include "KrimzLib/vecmath/vec4.h"

#include <iostream>

#include "KrimzLib/convert.h"


// Constructors
kl::vec4::vec4() {
	x = 0;
	y = 0;
	z = 0;
	w = 0;
}
kl::vec4::vec4(float a) {
	x = a;
	y = a;
	z = a;
	w = a;
}
kl::vec4::vec4(float x, float y, float z, float w) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}
kl::vec4::vec4(const kl::vec2& v, float z, float w) {
	x = v.x;
	y = v.y;
	this->z = z;
	this->w = w;
}
kl::vec4::vec4(float x, const kl::vec2& v, float w) {
	this->x = x;
	y = v.x;
	z = v.y;
	this->w = w;
}
kl::vec4::vec4(float x, float y, const kl::vec2& v) {
	this->x = x;
	this->y = y;
	z = v.x;
	w = v.y;
}
kl::vec4::vec4(const kl::vec2& v1, const kl::vec2& v2) {
	x = v1.x;
	y = v1.y;
	z = v2.x;
	w = v2.y;
}
kl::vec4::vec4(const kl::vec3& v) {
	x = v.x;
	y = v.y;
	z = v.z;
	w = 1;
}
kl::vec4::vec4(const kl::vec3& v, float w) {
	x = v.x;
	y = v.y;
	z = v.z;
	this->w = w;
}
kl::vec4::vec4(float x, const kl::vec3& v) {
	this->x = x;
	y = v.x;
	z = v.y;
	w = v.z;
}
kl::vec4::vec4(const kl::color& color) {
	this->kl::vec4::vec4(color, 1);
}

// Addition
kl::vec4 kl::vec4::add(const kl::vec4& obj) const {
	return kl::vec4(x + obj.x, y + obj.y, z + obj.z, w + obj.w);
}
kl::vec4 kl::vec4::operator+(const kl::vec4& obj) const {
	return this->add(obj);
}
void kl::vec4::operator+=(const kl::vec4& obj) {
	x += obj.x; y += obj.y; z += obj.z; w += obj.w;
}

// Subtraction
kl::vec4 kl::vec4::sub(const kl::vec4& obj) const {
	return kl::vec4(x - obj.x, y - obj.y, z - obj.z, w - obj.w);
}
kl::vec4 kl::vec4::operator-(const kl::vec4& obj) const {
	return this->sub(obj);
}
void kl::vec4::operator-=(const kl::vec4& obj) {
	x -= obj.x; y -= obj.y; z -= obj.z; w -= obj.w;
}

// Multiplication
kl::vec4 kl::vec4::mul(float a) const {
	return kl::vec4(x * a, y * a, z * a, w * a);
}
kl::vec4 kl::vec4::operator*(float a) const {
	return this->mul(a);
}
void kl::vec4::operator*=(float a) {
	x *= a; y *= a; z *= a; w *= a;
}
kl::vec4 kl::vec4::mul(const kl::vec4& obj) const {
	return kl::vec4(x * obj.x, y * obj.y, z * obj.z, w * obj.w);
}
kl::vec4 kl::vec4::operator*(const kl::vec4& obj) const {
	return this->mul(obj);
}
void kl::vec4::operator*=(const kl::vec4& obj) {
	x *= obj.x; y *= obj.y; z *= obj.z; w *= obj.w;
}

// Division
kl::vec4 kl::vec4::div(float a) const {
	return this->mul(1 / a);
}
kl::vec4 kl::vec4::operator/(float a) const {
	return this->div(a);
}
void kl::vec4::operator/=(float a) {
	this->operator*=(1 / a);
}
kl::vec4 kl::vec4::div(const kl::vec4& obj) const {
	return kl::vec4(x / obj.x, y / obj.y, z / obj.z, w / obj.w);
}
kl::vec4 kl::vec4::operator/(const kl::vec4& obj) const {
	return this->div(obj);
}
void kl::vec4::operator/=(const kl::vec4& obj) {
	x /= obj.x; y /= obj.y; z /= obj.z; w /= obj.w;
}

// Comparison
bool kl::vec4::equals(const kl::vec4& obj) const {
	return x == obj.x && y == obj.y && z == obj.z && w == obj.w;
}
bool kl::vec4::operator==(const kl::vec4& obj) const {
	return this->equals(obj);
}
bool kl::vec4::operator!=(const kl::vec4& obj) const {
	return !this->equals(obj);
}

// Returns a negated vector
kl::vec4 kl::vec4::negate() const {
	return this->mul(-1);
}

// Returns an absolute vector
kl::vec4 kl::vec4::abso() const {
	return kl::vec4(abs(x), abs(y), abs(z), abs(w));
}

// Returns the vectors length
float kl::vec4::length() const {
	return sqrt(x * x + y * y + z * z + w * w);
}

// Retruns a normalized vector
kl::vec4 kl::vec4::normalize() const {
	return this->div(this->length());
}

// Returns the dot product
float kl::vec4::dot(const kl::vec4& a) const {
	return x * a.x + y * a.y + z * a.z + w * a.w;
}

// Returns the angle between the given vector and self
float kl::vec4::angle(const kl::vec4& a) const {
	return kl::convert::toDegrees(acos(this->normalize().dot(a.normalize())));
}

// Overloading std::cout
std::ostream& kl::operator<<(std::ostream& os, const kl::vec4& obj) {
	os << kl::convert::toString(obj.x) << " " << kl::convert::toString(obj.y) << " " << kl::convert::toString(obj.z) << " " << kl::convert::toString(obj.w);
	return os;
}
