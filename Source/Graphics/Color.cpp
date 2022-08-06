#include "graphics/color.h"
#include "math/math.h"


kl::Color::Color() : b(0), g(0), r(0), a(255) {}

kl::Color::Color(byte r, byte g, byte b, byte a) : b(b), g(g), r(r), a(a) {}

bool kl::Color::operator==(const Color& obj) const {
	return r == obj.r && g == obj.g && b == obj.b && a == obj.a;
}

bool kl::Color::operator!=(const Color& obj) const {
	return !(*this == obj);
}

kl::Color kl::Color::asGray() const {
	const byte grayValue = byte(r * 0.3f + g * 0.585f + b * 0.115f);
	return Color(grayValue, grayValue, grayValue, a);
}

kl::Color kl::Color::asInverted() const {
	return Color(255 - r, 255 - g, 255 - b, a);
}

char kl::Color::asASCII() const {
	static const char asciiTable[10] = { '@', '%', '#', 'x', '+', '=', ':', '-', '.', ' ' };
	static const float toSatur = 9.0f / 255.0f;
	return asciiTable[int(asGray().r * toSatur)];
}

kl::Color kl::Color::mix(const Color& col, float ratio) const {
	ratio = Math::MinMax(ratio, 0.0f, 1.0f);
	return Color(
		byte(r * (1.0f - ratio)) + byte(col.r * ratio),
		byte(g * (1.0f - ratio)) + byte(col.g * ratio),
		byte(b * (1.0f - ratio)) + byte(col.b * ratio)
	);
}

kl::Color kl::Color::mix(const Color& col) const {
	return mix(col, ToFloatColor(col.a));
}

std::ostream& kl::operator<<(std::ostream& os, const Color& obj) {
	os << "\033[38;2;" << int(obj.r) << ";" << int(obj.g) << ";" << int(obj.b) << "m";
	return os;
}
