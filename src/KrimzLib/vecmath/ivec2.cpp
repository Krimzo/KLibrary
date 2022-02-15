#include "KrimzLib/vecmath/ivec2.h"

#include <iostream>

#include "KrimzLib/convert.h"


// Constructors
kl::ivec2::ivec2() {
	x = 0;
	y = 0;
}
kl::ivec2::ivec2(int x, int y) {
	this->x = x;
	this->y = y;
}

// Addition
kl::ivec2 kl::ivec2::add(const kl::ivec2& obj) const {
	return kl::ivec2(x + obj.x, y + obj.y);
}
kl::ivec2 kl::ivec2::operator+(const kl::ivec2& obj) const {
	return this->add(obj);
}
void kl::ivec2::operator+=(const kl::ivec2& obj) {
	x += obj.x; y += obj.y;
}

// Subtraction
kl::ivec2 kl::ivec2::sub(const kl::ivec2& obj) const {
	return kl::ivec2(x - obj.x, y - obj.y);
}
kl::ivec2 kl::ivec2::operator-(const kl::ivec2& obj) const {
	return this->sub(obj);
}
void kl::ivec2::operator-=(const kl::ivec2& obj) {
	x -= obj.x; y -= obj.y;
}

// Multiplication
kl::ivec2 kl::ivec2::mul(int a) const {
	return kl::ivec2(x * a, y * a);
}
kl::ivec2 kl::ivec2::operator*(int a) const {
	return this->mul(a);
}
void kl::ivec2::operator*=(int a) {
	x *= a; y *= a;
}
kl::ivec2 kl::ivec2::mul(const kl::ivec2& obj) const {
	return kl::ivec2(x * obj.x, y * obj.y);
}
kl::ivec2 kl::ivec2::operator*(const kl::ivec2& obj) const {
	return this->mul(obj);
}
void kl::ivec2::operator*=(const kl::ivec2& obj) {
	x *= obj.x; y *= obj.y;
}

// Division
kl::ivec2 kl::ivec2::div(int a) const {
	return kl::ivec2(x / a, y / a);
}
kl::ivec2 kl::ivec2::operator/(int a) const {
	return this->div(a);
}
void kl::ivec2::operator/=(int a) {
	x /= a; y /= a;
}
kl::ivec2 kl::ivec2::div(const kl::ivec2& obj) const {
	return kl::ivec2(x / obj.x, y / obj.y);
}
kl::ivec2 kl::ivec2::operator/(const kl::ivec2& obj) const {
	return this->div(obj);
}
void kl::ivec2::operator/=(const kl::ivec2& obj) {
	x /= obj.x; y /= obj.y;
}

// Comparison
bool kl::ivec2::equals(const kl::ivec2& obj) const {
	return x == obj.x && y == obj.y;
}
bool kl::ivec2::operator==(const kl::ivec2& obj) const {
	return this->equals(obj);
}
bool kl::ivec2::operator!=(const kl::ivec2& obj) const {
	return !this->equals(obj);
}

// Returns a negated vec
kl::ivec2 kl::ivec2::negate() const {
	return this->mul(-1);
}

// Returns an absolute vec
kl::ivec2 kl::ivec2::abso() const {
	return kl::ivec2(abs(x), abs(y));
}

// Overloading std::cout
std::ostream& kl::operator<<(std::ostream& os, const kl::ivec2& obj) {
	os << kl::convert::toString(obj.x) << " " << kl::convert::toString(obj.y);
	return os;
}
