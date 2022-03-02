#include "KrimzLib/math/int3.h"

#include <iostream>

#include "KrimzLib/utility/convert.h"


// Constructors
kl::int3::int3() {
	x = 0;
	y = 0;
	z = 0;
}
kl::int3::int3(int a) {
	x = a;
	y = a;
	z = a;
}
kl::int3::int3(int x, int y, int z) {
	this->x = x;
	this->y = y;
	this->z = z;
}
kl::int3::int3(const kl::int2& v, int z) {
	x = v.x;
	y = v.y;
	this->z = z;
}
kl::int3::int3(int x, const kl::int2& v) {
	this->x = x;
	y = v.x;
	z = v.y;
}

// Addition
kl::int3 kl::int3::add(const kl::int3& obj) const {
	return kl::int3(x + obj.x, y + obj.y, z + obj.z);
}
kl::int3 kl::int3::operator+(const kl::int3& obj) const {
	return this->add(obj);
}
void kl::int3::operator+=(const kl::int3& obj) {
	x += obj.x; y += obj.y; z += obj.z;
}

// Subtraction
kl::int3 kl::int3::sub(const kl::int3& obj) const {
	return kl::int3(x - obj.x, y - obj.y, z - obj.z);
}
kl::int3 kl::int3::operator-(const kl::int3& obj) const {
	return this->sub(obj);
}
void kl::int3::operator-=(const kl::int3& obj) {
	x -= obj.x; y -= obj.y; z -= obj.z;
}

// Multiplication
kl::int3 kl::int3::mul(int a) const {
	return kl::int3(x * a, y * a, z * a);
}
kl::int3 kl::int3::operator*(int a) const {
	return this->mul(a);
}
void kl::int3::operator*=(int a) {
	x *= a; y *= a; z *= a;
}
kl::int3 kl::int3::mul(const kl::int3& obj) const {
	return kl::int3(x * obj.x, y * obj.y, z * obj.z);
}
kl::int3 kl::int3::operator*(const kl::int3& obj) const {
	return this->mul(obj);
}
void kl::int3::operator*=(const kl::int3& obj) {
	x *= obj.x; y *= obj.y; z *= obj.z;
}

// Division
kl::int3 kl::int3::div(int a) const {
	return kl::int3(x / a, y / a, z / a);
}
kl::int3 kl::int3::operator/(int a) const {
	return this->div(a);
}
void kl::int3::operator/=(int a) {
	x /= a; y /= a; z /= a;
}
kl::int3 kl::int3::div(const kl::int3& obj) const {
	return kl::int3(x / obj.x, y / obj.y, z / obj.z);
}
kl::int3 kl::int3::operator/(const kl::int3& obj) const {
	return this->div(obj);
}
void kl::int3::operator/=(const kl::int3& obj) {
	x /= obj.x; y /= obj.y; z /= obj.z;
}

// Comparison
bool kl::int3::equals(const kl::int3& obj) const {
	return x == obj.x && y == obj.y && z == obj.z;
}
bool kl::int3::operator==(const kl::int3& obj) const {
	return this->equals(obj);
}
bool kl::int3::operator!=(const kl::int3& obj) const {
	return !this->equals(obj);
}

// Returns a negated vec
kl::int3 kl::int3::negate() const {
	return this->mul(-1);
}

// Returns an absolute vec
kl::int3 kl::int3::abso() const {
	return kl::int3(abs(x), abs(y), abs(z));
}

// Overloading std::cout
std::ostream& kl::operator<<(std::ostream& os, const kl::int3& obj) {
	os << "(" << kl::convert::toString(obj.x) << ", " << kl::convert::toString(obj.y) << ", " << kl::convert::toString(obj.z) << ")";
	return os;
}
