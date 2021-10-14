#pragma once
#include <iostream>
#include <vector>


/* --- TYPES --- */
namespace kl
{
	struct colorf
	{
		float r = 0;
		float g = 0;
		float b = 0;
		float a = 1;
	};

	struct colord
	{
		double r = 0;
		double g = 0;
		double b = 0;
		double a = 1;
	};

	struct color
	{
		byte r = 0;
		byte g = 0;
		byte b = 0;
		byte a = 255;

		// Returns the color as a float type
		colorf ToFloat() const
		{
			return { r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f };
		}

		// Returns the color as a double type
		colord ToDouble() const
		{
			return { r / 255.0, g / 255.0, b / 255.0, a / 255.0 };
		}
	};

	struct bitmap
	{
	public:
		// Constructor
		bitmap(int width, int height, color color = { 0, 0, 0, 255 })
		{
			this->width = width;
			this->height = height;
			pixels.resize(size_t(width) * size_t(height));
			FillSolid(color);
		}

		// Getters
		int GetWidth()
		{
			return width;
		}
		int GetHeight()
		{
			return height;
		}
		size_t GetSize()
		{
			return pixels.size();
		}
		color GetPixel(int x, int y)
		{
			if (x >= 0 && x < width && y >= 0 && y < height)
			{
				return pixels[y * size_t(width) + x];
			}
			return { 0, 0, 0 };
		}
		color* GetPixelData()
		{
			return &pixels[0];
		}

		// Setters
		void SetWidth(int width)
		{
			this->width = width;
			pixels.resize(size_t(this->width) * size_t(this->height));
		}
		void SetHeight(int height)
		{
			this->height = height;
			pixels.resize(size_t(this->width) * size_t(this->height));
		}
		void SetPixel(int x, int y, color color)
		{
			if (x >= 0 && x < width && y >= 0 && y < height)
			{
				pixels[y * size_t(width) + x] = color;
			}
		}

		// Fils the bitmap with solid color
		void FillSolid(color color)
		{
			std::fill(pixels.begin(), pixels.end(), color);
		}

		// Resets the byte values
		void FastClear(byte value)
		{
			memset(&pixels[0], value, pixels.size() * 4);
		}

	private:
		int width = 0;
		int height = 0;
		std::vector<color> pixels = {};
	};

	struct colorf_bgra
	{
		float b = 0;
		float g = 0;
		float r = 0;
		float a = 1;
	};

	struct colord_bgra
	{
		double b = 0;
		double g = 0;
		double r = 0;
		double a = 1;
	};

	struct color_bgra
	{
		byte b = 0;
		byte g = 0;
		byte r = 0;
		byte a = 255;

		// Returns the color as a float type
		colorf_bgra ToFloat() const
		{
			return { b / 255.0f, g / 255.0f, r / 255.0f, a / 255.0f };
		}

		// Returns the color as a double type
		colord_bgra ToDouble() const
		{
			return { b / 255.0, g / 255.0, r / 255.0, a / 255.0 };
		}
	};

	struct bitmap_bgra
	{
	public:
		// Constructor
		bitmap_bgra(int width, int height, color_bgra color = { 0, 0, 0, 255 })
		{
			this->width = width;
			this->height = height;
			pixels.resize(size_t(width) * size_t(height));
			FillSolid(color);
		}

		// Getters
		int GetWidth()
		{
			return width;
		}
		int GetHeight()
		{
			return height;
		}
		size_t GetSize()
		{
			return pixels.size();
		}
		color_bgra GetPixel(int x, int y)
		{
			if (x >= 0 && x < width && y >= 0 && y < height)
			{
				return pixels[y * size_t(width) + x];
			}
			return { 0, 0, 0 };
		}
		color_bgra* GetPixelData()
		{
			return &pixels[0];
		}

		// Setters
		void SetWidth(int width)
		{
			this->width = width;
			pixels.resize(size_t(this->width) * size_t(this->height));
		}
		void SetHeight(int height)
		{
			this->height = height;
			pixels.resize(size_t(this->width) * size_t(this->height));
		}
		void SetPixel(int x, int y, color_bgra color)
		{
			if (x >= 0 && x < width && y >= 0 && y < height)
			{
				pixels[y * size_t(width) + x] = color;
			}
		}

		// Fils the bitmap with solid color
		void FillSolid(color_bgra color)
		{
			std::fill(pixels.begin(), pixels.end(), color);
		}

		// Resets the byte values
		void FastClear(byte value)
		{
			memset(&pixels[0], value, pixels.size() * 4);
		}

	private:
		int width = 0;
		int height = 0;
		std::vector<color_bgra> pixels = {};
	};
}


/* --- CLASS --- */
namespace kl
{
	class graphics
	{
	public:
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
	};
}
