#include "KrimzLib/vecmath/vec2.h"

#include <iostream>

#include "KrimzLib/convert.h"


// Constructors
kl::vec2::vec2() {
	x = 0;
	y = 0;
}
kl::vec2::vec2(float a) {
	x = a;
	y = a;
}
kl::vec2::vec2(float x, float y) {
	this->x = x;
	this->y = y;
}
kl::vec2::vec2(const kl::ivec2& p) {
	x = float(p.x);
	y = float(p.y);
}
kl::vec2::vec2(const kl::vec2& a, const kl::vec2& b) {
	x = b.x - a.x;
	y = b.y - a.y;
}

// Addition
kl::vec2 kl::vec2::add(const kl::vec2& obj) const {
	return kl::vec2(x + obj.x, y + obj.y);
}
kl::vec2 kl::vec2::operator+(const kl::vec2& obj) const {
	return this->add(obj);
}
void kl::vec2::operator+=(const kl::vec2& obj) {
	x += obj.x; y += obj.y;
}

// Subtraction
kl::vec2 kl::vec2::sub(const kl::vec2& obj) const {
	return kl::vec2(x - obj.x, y - obj.y);
}
kl::vec2 kl::vec2::operator-(const kl::vec2& obj) const {
	return this->sub(obj);
}
void kl::vec2::operator-=(const kl::vec2& obj) {
	x -= obj.x; y -= obj.y;
}

// Multiplication
kl::vec2 kl::vec2::mul(float a) const {
	return kl::vec2(x * a, y * a);
}
kl::vec2 kl::vec2::operator*(float a) const {
	return this->mul(a);
}
void kl::vec2::operator*=(float a) {
	x *= a; y *= a;
}
kl::vec2 kl::vec2::mul(const kl::vec2& obj) const {
	return kl::vec2(x * obj.x, y * obj.y);
}
kl::vec2 kl::vec2::operator*(const kl::vec2& obj) const {
	return this->mul(obj);
}
void kl::vec2::operator*=(const kl::vec2& obj) {
	x *= obj.x; y *= obj.y;
}

// Division
kl::vec2 kl::vec2::div(float a) const {
	return this->mul(1 / a);
}
kl::vec2 kl::vec2::operator/(float a) const {
	return this->div(a);
}
void kl::vec2::operator/=(float a) {
	this->operator*=(1 / a);
}
kl::vec2 kl::vec2::div(const kl::vec2& obj) const {
	return kl::vec2(x / obj.x, y / obj.y);
}
kl::vec2 kl::vec2::operator/(const kl::vec2& obj) const {
	return this->div(obj);
}
void kl::vec2::operator/=(const kl::vec2& obj) {
	x /= obj.x; y /= obj.y;
}

// Comparison
bool kl::vec2::equals(const kl::vec2& obj) const {
	return x == obj.x && y == obj.y;
}
bool kl::vec2::operator==(const kl::vec2& obj) const {
	return this->equals(obj);
}
bool kl::vec2::operator!=(const kl::vec2& obj) const {
	return !this->equals(obj);
}

// Returns a negated vector
kl::vec2 kl::vec2::negate() const {
	return operator*(-1);
}

// Returns an absolute vector
kl::vec2 kl::vec2::abso() const {
	return kl::vec2(abs(x), abs(y));
}

// Returns the vectors length
float kl::vec2::length() const {
	return sqrt(x * x + y * y);
}

// Retruns a normalized vector
kl::vec2 kl::vec2::normalize() const {
	return this->div(this->length());
}

// Returns the dot product
float kl::vec2::dot(const vec2& a) const {
	return x * a.x + y * a.y;
}

// Returns the angle between the given vector and self
float kl::vec2::angle(const kl::vec2& a, bool full) const {
	if (full) {
		return kl::convert::toDegrees(atan2(x * a.y - y * a.x, x * a.x + y * a.y));
	}
	return kl::convert::toDegrees(acos(this->normalize().dot(a.normalize())));
}

// Returns a rotated vector around the given point
kl::vec2 kl::vec2::rotate(float angle) const {
	const float sinA = sin(kl::convert::toRadians(angle));
	const float cosA = cos(kl::convert::toRadians(angle));
	return kl::vec2(cosA * x - sinA * y, sinA * x + cosA * y);
}

// Constants
const kl::vec2 kl::vec2::pos_x = kl::vec2( 1.0f,  0.0f);
const kl::vec2 kl::vec2::neg_x = kl::vec2(-1.0f,  0.0f);
const kl::vec2 kl::vec2::pos_y = kl::vec2( 0.0f,  1.0f);
const kl::vec2 kl::vec2::neg_y = kl::vec2( 0.0f, -1.0f);

// Overloading std::cout
std::ostream& kl::operator<<(std::ostream& os, const kl::vec2& obj) {
	os << kl::convert::toString(obj.x) << " " << kl::convert::toString(obj.y);
	return os;
}
