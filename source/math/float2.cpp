#include "math/float2.h"

#include <iostream>

#include "utility/convert.h"


// Constr
kl::float2::float2() : x(0.0f), y(0.0f) {}
kl::float2::float2(float a) : x(a), y(a) {}
kl::float2::float2(float x, float y) : x(x), y(y) {}
kl::float2::float2(const kl::int2& v) : x(float(v.x)), y(float(v.y)) {}

// Addition
kl::float2 kl::float2::add(const kl::float2& obj) const {
	return kl::float2(x + obj.x, y + obj.y);
}
kl::float2 kl::float2::operator+(const kl::float2& obj) const {
	return add(obj);
}
void kl::float2::operator+=(const kl::float2& obj) {
	x += obj.x; y += obj.y;
}

// Subtraction
kl::float2 kl::float2::sub(const kl::float2& obj) const {
	return kl::float2(x - obj.x, y - obj.y);
}
kl::float2 kl::float2::operator-(const kl::float2& obj) const {
	return sub(obj);
}
void kl::float2::operator-=(const kl::float2& obj) {
	x -= obj.x; y -= obj.y;
}

// Multiplication
kl::float2 kl::float2::mul(float a) const {
	return kl::float2(x * a, y * a);
}
kl::float2 kl::float2::operator*(float a) const {
	return mul(a);
}
void kl::float2::operator*=(float a) {
	x *= a; y *= a;
}
kl::float2 kl::float2::mul(const kl::float2& obj) const {
	return kl::float2(x * obj.x, y * obj.y);
}
kl::float2 kl::float2::operator*(const kl::float2& obj) const {
	return mul(obj);
}
void kl::float2::operator*=(const kl::float2& obj) {
	x *= obj.x; y *= obj.y;
}

// Division
kl::float2 kl::float2::div(float a) const {
	return mul(1 / a);
}
kl::float2 kl::float2::operator/(float a) const {
	return div(a);
}
void kl::float2::operator/=(float a) {
	operator*=(1 / a);
}
kl::float2 kl::float2::div(const kl::float2& obj) const {
	return kl::float2(x / obj.x, y / obj.y);
}
kl::float2 kl::float2::operator/(const kl::float2& obj) const {
	return div(obj);
}
void kl::float2::operator/=(const kl::float2& obj) {
	x /= obj.x; y /= obj.y;
}

// Comparison
bool kl::float2::equals(const kl::float2& obj) const {
	return x == obj.x && y == obj.y;
}
bool kl::float2::operator==(const kl::float2& obj) const {
	return equals(obj);
}
bool kl::float2::operator!=(const kl::float2& obj) const {
	return !equals(obj);
}

// Returns a negated vector
kl::float2 kl::float2::negate() const {
	return operator*(-1);
}

// Returns an absolute vector
kl::float2 kl::float2::abso() const {
	return kl::float2(abs(x), abs(y));
}

// Returns the vectors length
float kl::float2::length() const {
	return sqrt(x * x + y * y);
}

// Retruns a normalized vector
kl::float2 kl::float2::normalize() const {
	return div(length());
}

// Returns the dot product
float kl::float2::dot(const float2& a) const {
	return x * a.x + y * a.y;
}

// Returns the angle between the given vector and self
float kl::float2::angle(const kl::float2& a, bool full) const {
	if (full) {
		return kl::convert::toDegrees(atan2(x * a.y - y * a.x, x * a.x + y * a.y));
	}
	return kl::convert::toDegrees(acos(normalize().dot(a.normalize())));
}

// Returns a rotated vector around the given point
kl::float2 kl::float2::rotate(float angle) const {
	const float sinA = sin(kl::convert::toRadians(angle));
	const float cosA = cos(kl::convert::toRadians(angle));
	return kl::float2(cosA * x - sinA * y, sinA * x + cosA * y);
}

// Constants
const kl::float2 kl::float2::pos_x = kl::float2(1.0f, 0.0f);
const kl::float2 kl::float2::neg_x = kl::float2(-1.0f, 0.0f);
const kl::float2 kl::float2::pos_y = kl::float2(0.0f, 1.0f);
const kl::float2 kl::float2::neg_y = kl::float2(0.0f, -1.0f);

// Overloading std::cout
std::ostream& kl::operator<<(std::ostream& os, const kl::float2& obj) {
	os << "(" << kl::convert::toString(obj.x) << ", " << kl::convert::toString(obj.y) << ")";
	return os;
}
