#define _CRT_SECURE_NO_WARNINGS
#include "KrimzLib/convert.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

#include "KrimzLib/math.h"
#include "KrimzLib/vecmath/vec3.h"
#include "KrimzLib/graphics/color.h"


// Converts degrees to radians
float kl::convert::toRadians(float degrees) {
	static float toRadians = kl::math::pi / 180;
	return degrees * toRadians;
}

// Converts radians to degrees
float kl::convert::toDegrees(float radians) {
	static float toDegrees = 180 / kl::math::pi;
	return radians * toDegrees;
}

// Converts a float color to color
kl::color kl::convert::toColor(const kl::vec3& colf) {
	return kl::color(byte(colf.x * 255), byte(colf.y * 255), byte(colf.z * 255));
}

// Converts a string to a wstring
std::wstring kl::convert::toWString(const std::string& data) {
	std::wstring toReturn;
	toReturn.resize(data.size());
	mbstowcs(&toReturn[0], &data[0], data.size());
	return toReturn;
}

// Alligns the minus sign
std::string kl::convert::toString(int a) {
	std::stringstream ss;
	ss << std::setw(5) << a;
	return ss.str();
}
std::string kl::convert::toString(float a) {
	std::stringstream ss;
	ss << std::fixed << std::setprecision(2) << std::setw(8) << a;
	return ss.str();
}
