#pragma once


namespace kl {
	namespace convert {
		// Converts degrees to radians
		float toRadians(float degrees) {
			static float toRadians = kl::constant::floats::pi / 180;
			return degrees * toRadians;
		}

		// Converts radians to degrees
		float toDegrees(float radians) {
			static float toDegrees = 180 / kl::constant::floats::pi;
			return radians * toDegrees;
		}

		// Converts a float color to color
		kl::color toColor(kl::vec3 colf) {
			return kl::color(kl::byte(colf.x * 255), kl::byte(colf.y * 255), kl::byte(colf.z * 255));
		}
		
		// Converts a string to a wstring
		std::wstring toWString(std::string data) {
			std::wstring toReturn;
			toReturn.resize(data.size());
			mbstowcs(&toReturn[0], &data[0], data.size());
			return toReturn;
		}
	};
}
