#pragma once


namespace kl {
	class convert {
	public:
		// Converts radians to degrees
		static double ToDegrees(double radians) {
			return radians * toDegrees;
		}

		// Converts degrees to radians
		static double ToRadians(double degrees) {
			return degrees * toRadians;
		}

		// Converts a byte array to a string
		static std::string ToString(bytes& b) {
			std::string stringBuffer;
			stringBuffer.resize(b.size());
			memcpy(&stringBuffer[0], &b[0], b.size());
			return stringBuffer;
		}
		static std::string ToString(bytes&& b) {
			std::string stringBuffer;
			stringBuffer.resize(b.size());
			memcpy(&stringBuffer[0], &b[0], b.size());
			return stringBuffer;
		}

		// Converts a string to a bytes array
		static bytes ToBytes(std::string& s) {
			bytes tempBytes(s.size());
			memcpy(&tempBytes[0], &s[0], s.size());
			return tempBytes;
		}
		static bytes ToBytes(std::string&& s) {
			bytes tempBytes(s.size());
			memcpy(&tempBytes[0], &s[0], s.size());
			return tempBytes;
		}

	private:
		static double toRadians;
		static double toDegrees;
		static float toFloatColor;
		static double toDoubleColor;
	};
	double convert::toRadians = constant::pi / 180;
	double convert::toDegrees = 180 / constant::pi;
	float convert::toFloatColor = 1 / 255.0f;
	double convert::toDoubleColor = 1 / 255.0;
}
