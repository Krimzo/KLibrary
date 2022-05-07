#include "math/int2.h"

#include <iostream>

#include "utility/convert.h"


// Constr
kl::int2::int2() : x(0), y(0) {}
kl::int2::int2(int a) : x(a), y(a) {}
kl::int2::int2(int x, int y) : x(x), y(y) {}

// Getters
int& kl::int2::operator[](int i) {
	return data[i];
}
const int& kl::int2::operator[](int i) const {
	return data[i];
}

// Addition
kl::int2 kl::int2::add(const kl::int2& obj) const {
	return kl::int2(x + obj.x, y + obj.y);
}
kl::int2 kl::int2::operator+(const kl::int2& obj) const {
	return add(obj);
}
void kl::int2::operator+=(const kl::int2& obj) {
	x += obj.x; y += obj.y;
}

// Subtraction
kl::int2 kl::int2::sub(const kl::int2& obj) const {
	return kl::int2(x - obj.x, y - obj.y);
}
kl::int2 kl::int2::operator-(const kl::int2& obj) const {
	return sub(obj);
}
void kl::int2::operator-=(const kl::int2& obj) {
	x -= obj.x; y -= obj.y;
}

// Multiplication
kl::int2 kl::int2::mul(int a) const {
	return kl::int2(x * a, y * a);
}
kl::int2 kl::int2::operator*(int a) const {
	return mul(a);
}
void kl::int2::operator*=(int a) {
	x *= a; y *= a;
}
kl::int2 kl::int2::mul(const kl::int2& obj) const {
	return kl::int2(x * obj.x, y * obj.y);
}
kl::int2 kl::int2::operator*(const kl::int2& obj) const {
	return mul(obj);
}
void kl::int2::operator*=(const kl::int2& obj) {
	x *= obj.x; y *= obj.y;
}

// Division
kl::int2 kl::int2::div(int a) const {
	return kl::int2(x / a, y / a);
}
kl::int2 kl::int2::operator/(int a) const {
	return div(a);
}
void kl::int2::operator/=(int a) {
	x /= a; y /= a;
}
kl::int2 kl::int2::div(const kl::int2& obj) const {
	return kl::int2(x / obj.x, y / obj.y);
}
kl::int2 kl::int2::operator/(const kl::int2& obj) const {
	return div(obj);
}
void kl::int2::operator/=(const kl::int2& obj) {
	x /= obj.x; y /= obj.y;
}

// Comparison
bool kl::int2::equals(const kl::int2& obj) const {
	return x == obj.x && y == obj.y;
}
bool kl::int2::operator==(const kl::int2& obj) const {
	return equals(obj);
}
bool kl::int2::operator!=(const kl::int2& obj) const {
	return !equals(obj);
}

// Returns a negated vec
kl::int2 kl::int2::negate() const {
	return mul(-1);
}

// Returns an absolute vec
kl::int2 kl::int2::abso() const {
	return kl::int2(abs(x), abs(y));
}

// Overloading std::cout
std::ostream& kl::operator<<(std::ostream& os, const kl::int2& obj) {
	os << "(" << obj.x << ", " << obj.y << ")";
	return os;
}
