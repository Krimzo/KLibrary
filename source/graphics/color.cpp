#include "graphics/color.h"


// Constructors
kl::color::color() : b(0), g(0), r(0), a(0) {}
kl::color::color(byte r, byte g, byte b, byte a) : b(b), g(g), r(r), a(a) {}

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

int kl::color::getInt() const {
	return *(int*)this;
}
kl::color kl::color::grayscale() const {
	const byte grayValue = byte(r * 0.299f + g * 0.587f + b * 0.114f);
	return kl::color(grayValue, grayValue, grayValue, a);
}
kl::color kl::color::invert() const {
	return kl::color(255 - r, 255 - g, 255 - b, a);
}
char kl::color::toASCII() const {
	static const char asciiTable[10] = { '@', '%', '#', 'x', '+', '=', ':', '-', '.', ' ' };
	static const float toSatur = 9.0f / 255.0f;
	return asciiTable[int(grayscale().r * toSatur)];
}

kl::color kl::color::mix(const kl::color& col, float ratio) const {
	// Calc r and ir
	ratio = max(min(ratio, 1.0f), 0.0f);
	const float iratio = 1.0f - ratio;

	// Calculating mixed color
	return kl::color(
		byte(r * iratio) + byte(col.r * ratio),
		byte(g * iratio) + byte(col.g * ratio),
		byte(b * iratio) + byte(col.b * ratio)
	);
}
kl::color kl::color::mix(const kl::color& col) const {
	static const float toFloatCol = 1.0f / 255;
	return mix(col, col.a * toFloatCol);
}

// Overloading std::cout
std::ostream& kl::operator<<(std::ostream& os, const kl::color& obj) {
	os << "\033[38;2;" << int(obj.r) << ";" << int(obj.g) << ";" << int(obj.b) << "m";
	return os;
}
