#pragma once


namespace kl {
	class convert {
	public:
		// Converts degrees to radians
		static double ToRadians(double degrees) {
			return degrees * constant::toRadians;
		}

		// Converts radians to degrees
		static double ToDegrees(double radians) {
			return radians * constant::toDegrees;
		}

		// Converts a byte array to a string
		static std::string ToString(bytes& b) {
			std::string stringBuffer;
			stringBuffer.resize(b.size());
			memcpy(&stringBuffer[0], &b[0], b.size());
			return stringBuffer;
		}
		static std::string ToString(bytes&& b) {
			ToString(b);
		}

		// Converts a string to a bytes array
		static bytes ToBytes(std::string& s) {
			bytes tempBytes(s.size());
			memcpy(&tempBytes[0], &s[0], s.size());
			return tempBytes;
		}
		static bytes ToBytes(std::string&& s) {
			ToBytes(s);
		}

		// Converts a string to a wstring
		static std::wstring ToWString(std::string data) {
			std::wstring toReturn;
			toReturn.resize(data.size());
			mbstowcs(&toReturn[0], &data[0], data.size());
			return toReturn;
		}
	};
}
