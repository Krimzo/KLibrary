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
float kl::vec2::angle(const kl::vec2& a) const {
	return kl::convert::toDegrees(acos(this->normalize().dot(a.normalize())));
}

// Returns a rotated vector around the given point
kl::vec2 kl::vec2::rotate(float angle) const {
	const float sinA = sin(kl::convert::toRadians(angle));
	const float cosA = cos(kl::convert::toRadians(angle));
	return kl::vec2(cosA * x - sinA * y, sinA * x + cosA * y);
}

// Prints the data to the console
void kl::vec2::print() const {
	printf("% .2f % .2f\n", x, y);
}
