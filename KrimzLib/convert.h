#pragma once
#include <sstream>
#include "KrimzLib/types.h"
#include "KrimzLib/constant.h"


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

		// Converts a color/bitmap from RGBA to BGRA
		static colorf_bgra ToBGRA(colorf c)
		{
			return { c.b, c.g, c.r, c.a };
		}
		static colord_bgra ToBGRA(colord c)
		{
			return { c.b, c.g, c.r, c.a };
		}
		static color_bgra ToBGRA(color c)
		{
			return { c.b, c.g, c.r, c.a };
		}
		static bitmap_bgra ToBGRA(bitmap& bm)
		{
			bitmap_bgra tempBM(bm.GetWidth(), bm.GetHeight());
			for (int i = 0; i < bm.GetSize(); i++)
			{
				tempBM.GetPixelData()[i] = ToBGRA(bm.GetPixelData()[i]);
			}
			return tempBM;
		}
		static bitmap_bgra ToBGRA(bitmap&& bm)
		{
			bitmap_bgra tempBM(bm.GetWidth(), bm.GetHeight());
			for (int i = 0; i < bm.GetSize(); i++)
			{
				tempBM.GetPixelData()[i] = ToBGRA(bm.GetPixelData()[i]);
			}
			return tempBM;
		}

		// Converts a color/bitmap from BGRA to RGBA
		static colorf ToRGBA(colorf_bgra c)
		{
			return { c.r, c.g, c.b, c.a };
		}
		static colord ToRGBA(colord_bgra c)
		{
			return { c.r, c.g, c.b, c.a };
		}
		static color ToRGBA(color_bgra c)
		{
			return { c.r, c.g, c.b, c.a };
		}
		static bitmap ToRGBA(bitmap_bgra& bm)
		{
			bitmap tempBM(bm.GetWidth(), bm.GetHeight());
			for (int i = 0; i < bm.GetSize(); i++)
			{
				tempBM.GetPixelData()[i] = ToRGBA(bm.GetPixelData()[i]);
			}
			return tempBM;
		}
		static bitmap ToRGBA(bitmap_bgra&& bm)
		{
			bitmap tempBM(bm.GetWidth(), bm.GetHeight());
			for (int i = 0; i < bm.GetSize(); i++)
			{
				tempBM.GetPixelData()[i] = ToRGBA(bm.GetPixelData()[i]);
			}
			return tempBM;
		}

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
		static std::string ToString(bytes&& b)
		{
			std::stringstream ss;
			for (int i = 0; i < b.size(); i++)
			{
				ss << (char)b[i];
			}
			return ss.str();
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
