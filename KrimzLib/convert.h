#pragma once


namespace kl
{
	class convert
	{
	public:
		// Converts radians to degrees
		static double ToDegrees(double radians)
		{
			return radians * toDegrees;
		}

		// Converts degrees to radians
		static double ToRadians(double degrees)
		{
			return degrees * toRadians;
		}

		// Converts the color to a float type
		static colorf ToFloatColor(color c)
		{
			return { c.r * toFloatColor, c.g * toFloatColor, c.b * toFloatColor, c.a * toFloatColor };
		}

		// Converts the color tp a double type
		static colord ToDoubleColor(color c)
		{
			return { c.r * toDoubleColor, c.g * toDoubleColor, c.b * toDoubleColor, c.a * toDoubleColor };
		}

		// Converts the color to a byte type
		static color ToByteColor(colorf c)
		{
			return { byte(c.r * 255), byte(c.g * 255), byte(c.b * 255), byte(c.a * 255) };
		}
		static color ToByteColor(colord c)
		{
			return { byte(c.r * 255), byte(c.g * 255), byte(c.b * 255), byte(c.a * 255) };
		}

		// Converts a byte array to a string
		static std::string ToString(bytes& b)
		{
			std::stringstream ss;
			for (int i = 0; i < b.size(); i++)
			{
				if (b[i])
					ss << b[i];
				else
					break;
			}
			return ss.str();
		}
		static std::string ToString(bytes&& b)
		{
			std::stringstream ss;
			for (int i = 0; i < b.size(); i++)
			{
				if (b[i])
					ss << b[i];
				else
					break;
			}
			return ss.str();
		}

		// Converts a string to a bytes array
		static bytes ToBytes(std::string& s)
		{
			bytes tempBytes(s.size());
			memcpy(&tempBytes[0], &s[0], s.size());
			return tempBytes;
		}
		static bytes ToBytes(std::string&& s)
		{
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
