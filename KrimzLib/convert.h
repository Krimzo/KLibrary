#pragma once


namespace kl {
	class convert {
	public:
		// Converts degrees to radians
		static double ToRadians(double degrees) {
			return degrees * constant::doubles::toRadians;
		}

		// Converts radians to degrees
		static double ToDegrees(double radians) {
			return radians * constant::doubles::toDegrees;
		}

		// Converts a byte array to a string
		static std::string ToString(bytes& data) {
			std::string stringBuffer;
			stringBuffer.resize(data.size());
			memcpy(&stringBuffer[0], &data[0], data.size());
			return stringBuffer;
		}
		static std::string ToString(bytes&& data) {
			return ToString(data);
		}

		// Converts a string to a bytes array
		static bytes ToBytes(std::string& data) {
			bytes tempBytes(data.size());
			memcpy(&tempBytes[0], &data[0], data.size());
			return tempBytes;
		}
		static bytes ToBytes(std::string&& data) {
			return ToBytes(data);
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
