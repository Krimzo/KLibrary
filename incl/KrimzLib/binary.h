#pragma once


namespace kl {
	class binary {
	public:
		// Returns bytes from the given value
		template<typename T> static kl::bytes ToBytes(T num) {
			kl::byte* bytePointer(&num);
			std::vector<byte> byteData;
			for (int i = 0; i < sizeof(num); i++) {
				byteData.insert(byteData.begin(), *(bytePointer + i));
			}
			return byteData;
		}

		// Returns bits from the given value
		template<typename T> static kl::bits ToBits(T num) {
			kl::byte* bytePointer(&num);
			std::string bitData = "";
			for (int i = 0; i < sizeof(num); i++) {
				byte tempByte = *(bytePointer + i);
				for (int j = 0; j < 8; j++) {
					bitData.insert(bitData.begin(), char(tempByte % 2 + '0'));
					tempByte /= 2;
				}
			}
			return bitData;
		}
	};
}
