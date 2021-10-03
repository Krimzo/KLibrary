#pragma once
#include <iostream>


namespace kl
{
	class binary
	{
	public:
		// Returns a bits from the given value
		static std::string GetBits(char num)
		{
			unsigned char tempNum = *(unsigned char*)&num;
			std::string data;
			for (int i = 0; i < 8; i++)
			{
				if (tempNum)
				{
					data.insert(data.begin(), char(tempNum % 2 + 48));
					tempNum /= 2;
				}
				else
				{
					data.insert(data.begin(), '0');
				}
			}
			return data;
		}
		static std::string GetBits(short num)
		{
			unsigned short tempNum = *(unsigned short*)&num;
			std::string data;
			for (int i = 0; i < 16; i++)
			{
				if (tempNum)
				{
					data.insert(data.begin(), char(tempNum % 2 + 48));
					tempNum /= 2;
				}
				else
				{
					data.insert(data.begin(), '0');
				}
			}
			return data;
		}
		static std::string GetBits(int num)
		{
			unsigned int tempNum = *(unsigned int*)&num;
			std::string data;
			for (int i = 0; i < 32; i++)
			{
				if (tempNum)
				{
					data.insert(data.begin(), char(tempNum % 2 + 48));
					tempNum /= 2;
				}
				else
				{
					data.insert(data.begin(), '0');
				}
					
			}
			return data;
		}
		static std::string GetBits(float num)
		{
			unsigned int tempNum = *(unsigned int*)&num;
			std::string data;
			for (int i = 0; i < 32; i++)
			{
				if (tempNum)
				{
					data.insert(data.begin(), char(tempNum % 2 + 48));
					tempNum /= 2;
				}
				else
				{
					data.insert(data.begin(), '0');
				}
			}
			return data;
		}
		static std::string GetBits(long long num)
		{
			unsigned long long tempNum = *(unsigned long long*)&num;
			std::string data;
			for (int i = 0; i < 64; i++)
			{
				if (tempNum)
				{
					data.insert(data.begin(), char(tempNum % 2 + 48));
					tempNum /= 2;
				}
				else
				{
					data.insert(data.begin(), '0');
				}
			}
			return data;
		}
		static std::string GetBits(double num)
		{
			unsigned long long tempNum = *(unsigned long long*)&num;
			std::string data;
			for (int i = 0; i < 64; i++)
			{
				if (tempNum)
				{
					data.insert(data.begin(), char(tempNum % 2 + 48));
					tempNum /= 2;
				}
				else
				{
					data.insert(data.begin(), '0');
				}
			}
			return data;
		}
	};
}
