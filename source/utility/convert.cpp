#define _CRT_SECURE_NO_WARNINGS
#include "utility/convert.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

#include "math/math.h"
#include "math/float4.h"


// Converts degrees to radians
float kl::convert::toRadians(float degrees) {
	static const float toRadians = kl::math::pi / 180.0f;
	return degrees * toRadians;
}
kl::float2 kl::convert::toRadians(const kl::float2& degrees) {
	return kl::float2(toRadians(degrees.x), toRadians(degrees.y));
}
kl::float3 kl::convert::toRadians(const kl::float3& degrees) {
	return kl::float3(toRadians(degrees.x), toRadians(degrees.y), toRadians(degrees.z));
}

// Converts radians to degrees
float kl::convert::toDegrees(float radians) {
	static const float toDegrees = 180.0f / kl::math::pi;
	return radians * toDegrees;
}
kl::float2 kl::convert::toDegrees(const kl::float2& radians) {
	return kl::float2(toDegrees(radians.x), toDegrees(radians.y));
}
kl::float3 kl::convert::toDegrees(const kl::float3& radians) {
	return kl::float3(toDegrees(radians.x), toDegrees(radians.y), toDegrees(radians.z));
}

// Byte to float color
float kl::convert::toFloCol(byte val) {
	static const float conv = 1.0f / 255.0f;
	return val * conv;
}

// Converts a float color to color
kl::color kl::convert::toColor(const kl::float4& colf) {
	return kl::color(byte(min(max(colf.x, 0.0f), 1.0f) * 255), byte(min(max(colf.y, 0.0f), 1.0f) * 255), byte(min(max(colf.z, 0.0f), 1.0f) * 255), byte(min(max(colf.w, 0.0f), 1.0f) * 255));
}

// Converts a string to a wstring
std::wstring kl::convert::toWString(const String& data) {
	std::wstring toReturn;
	toReturn.resize(data.size());
	mbstowcs(&toReturn[0], &data[0], data.size());
	return toReturn;
}
