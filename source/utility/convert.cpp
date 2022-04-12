#define _CRT_SECURE_NO_WARNINGS
#include "utility/convert.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

#include "math/math.h"
#include "math/float4.h"
#include "color/color.h"


// Converts degrees to radians
float kl::convert::toRadians(float degrees)
{
	static const float toRadians = kl::math::pi / 180.0f;
	return degrees * toRadians;
}

// Converts radians to degrees
float kl::convert::toDegrees(float radians)
{
	static const float toDegrees = 180.0f / kl::math::pi;
	return radians * toDegrees;
}

// Byte to float color
float kl::convert::toFloCol(byte val)
{
	static const float conv = 1.0f / 255.0f;
	return val * conv;
}

// Converts a float color to color
kl::color kl::convert::toColor(const kl::float4& colf)
{
	return kl::color(byte(colf.x * 255), byte(colf.y * 255), byte(colf.z * 255), byte(colf.w * 255));
}

// Converts a string to a wstring
std::wstring kl::convert::toWString(const std::string& data)
{
	std::wstring toReturn;
	toReturn.resize(data.size());
	mbstowcs(&toReturn[0], &data[0], data.size());
	return toReturn;
}

// Alligns the minus sign
std::string kl::convert::toString(int a)
{
	std::stringstream ss;
	ss << std::setw(5) << a;
	return ss.str();
}
std::string kl::convert::toString(float a)
{
	std::stringstream ss;
	ss << std::fixed << std::setprecision(2) << std::setw(8) << a;
	return ss.str();
}
