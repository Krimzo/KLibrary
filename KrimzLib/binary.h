#pragma once


namespace kl {
	class binary {
	public:
		// Returns bits from the given value
		template<typename T> static bits GetBits(T num) {
			byte* bytePointer = (byte*)&num;
			std::string bitData = "";
			for (int i = 0; i < sizeof(num); i++) {
				byte tempByte = *(bytePointer + i);
				for (int j = 0; j < 8; j++) {
					bitData.insert(bitData.begin(), char(tempByte % 2 + 48));
					tempByte /= 2;
				}
			}
			return bitData;
		}

		// Returns bytes from the given value
		template<typename T> static bytes GetBytes(T num) {
			byte* bytePointer = (byte*)&num;
			std::vector<byte> byteData;
			for (int i = 0; i < sizeof(num); i++) {
				byteData.insert(byteData.begin(), *(bytePointer + i));
			}
			return byteData;
		}
	};
}
