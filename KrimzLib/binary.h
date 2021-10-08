#pragma once
#include <iostream>
#include <sstream>


namespace kl
{
	class binary
	{
	public:
		// Converts a byte array to a string
		static std::string ToString(bytes& b)
		{
			std::stringstream ss;
			for (int i = 0; i < b.size(); i++)
			{
				ss << (char)b[i];
			}
			return ss.str();
		}

		// Converts a byte to the hex string
		static std::string ToHex(byte b)
		{
			char tempBuffer[5];
			sprintf_s(tempBuffer, "0x%02X", b);
			return tempBuffer;
		}

		// Returns bits from the given value
		template<typename T> static bits GetBits(T num)
		{
			byte* bytePointer = (byte*)&num;
			std::string bitData = "";
			for (int i = 0; i < sizeof(num); i++)
			{
				byte tempByte = *(bytePointer + i);
				for (int j = 0; j < 8; j++)
				{
					bitData.insert(bitData.begin(), char(tempByte % 2 + 48));
					tempByte /= 2;
				}
			}
			return bitData;
		}

		// Returns bytes from the given value
		template<typename T> static bytes GetBytes(T num)
		{
			byte* bytePointer = (byte*)&num;
			std::vector<byte> byteData;
			for (int i = 0; i < sizeof(num); i++)
			{
				byteData.insert(byteData.begin(), *(bytePointer + i));
			}
			return byteData;
		}
	};
}
