#pragma once

#include <string>

#include "libspec/using.h"
#include "math/float4.h"
#include "graphics/color.h"


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
		std::wstring toWString(const String& data);

		// Alligns the minus sign
		String toString(int a);
		String toString(float a);
	};
}
