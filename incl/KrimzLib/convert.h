#pragma once


namespace kl {
	namespace convert {
		// Converts degrees to radians
		float toRadians(float degrees) {
			return degrees * kl::constant::floats::toRadians;
		}

		// Converts radians to degrees
		float toDegrees(float radians) {
			return radians * kl::constant::floats::toDegrees;
		}

		// Converts a byte array to a string
		std::string toString(kl::bytes& data) {
			std::stringstream ss;
			for (kl::uint64 i = 0; i < data.size(); i++) {
				ss << (char)data[i];
			}
			return ss.str();
		}
		std::string toString(kl::bytes&& data) {
			return toString(data);
		}

		// Converts a string to a bytes array
		kl::bytes toBytes(std::string& data) {
			kl::bytes tempBytes(data.size());
			for (kl::uint64 i = 0; i < data.size(); i++) {
				tempBytes[i] = (byte)data[i];
			}
			return tempBytes;
		}
		kl::bytes toBytes(std::string&& data) {
			return toBytes(data);
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
