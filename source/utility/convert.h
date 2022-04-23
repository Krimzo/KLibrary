#pragma once

#include <string>

#include "math/float4.h"
#include "color/color.h"


namespace kl {
	namespace convert {
		// Converts degrees to radians
		float toRadians(float degrees);

		// Converts radians to degrees
		float toDegrees(float radians);

		// Byte to float color
		float toFloCol(byte val);

		// Converts a float color to color
		kl::color toColor(const kl::float4& colf);

		// Converts a string to a wstring
		std::wstring toWString(const std::string& data);

		// Alligns the minus sign
		std::string toString(int a);
		std::string toString(float a);
	};
}
