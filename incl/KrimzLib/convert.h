#pragma once

#include <string>

#include "KrimzLib/vecmath/vec3.h"
#include "KrimzLib/graphics/color.h"


namespace kl {
	namespace convert {
		// Converts degrees to radians
		float toRadians(float degrees);

		// Converts radians to degrees
		float toDegrees(float radians);

		// Converts a float color to color
		kl::color toColor(const kl::vec3& colf);
		
		// Converts a string to a wstring
		std::wstring toWString(const std::string& data);

		// Alligns the minus sign
		std::string toString(int a);
		std::string toString(float a);
	};
}
