#include "KrimzLib/color/color.h"


// Constructors
kl::color::color() {
	r = 0;
	g = 0;
	b = 0;
	a = 255;
}
kl::color::color(byte r, byte g, byte b, byte a) {
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

// Operator overloading
bool kl::color::equals(const kl::color& obj) const {
	return r == obj.r && g == obj.g && b == obj.b && a == obj.a;
}
bool kl::color::operator==(const kl::color& obj) const {
	return this->equals(obj);
}
bool kl::color::operator!=(const kl::color& obj) const {
	return !this->equals(obj);
}

// Returns data as an int
int kl::color::getInt() const {
	return *(int*)this;
}

// Returns grayscaled color
kl::color kl::color::grayscale() const {
	const byte grayValue = byte(r * 0.299f + g * 0.587f + b * 0.114f);
	return kl::color(grayValue, grayValue, grayValue);
}

// Overloading std::cout
std::ostream& kl::operator<<(std::ostream& os, const kl::color& obj) {
	os << "\033[38;2;" << int(obj.r) << ";" << int(obj.g) << ";" << int(obj.b) << "m";
	return os;
}
