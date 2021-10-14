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

		// Prints the color to the console
		void Print() const
		{
			printf("r: %lf g: %lf b: %lf a: %lf\n", r, g, b, a);
		}
	};

	struct colord
	{
		double r = 0;
		double g = 0;
		double b = 0;
		double a = 1;

		// Prints the color to the console
		void Print() const
		{
			printf("r: %lf g: %lf b: %lf a: %lf\n", r, g, b, a);
		}
	};

	struct color
	{
		/* Had to flip red and blue channels in order to fix StretchDIBits function */
		byte b;
		byte g;
		byte r;
		byte a;

		color(byte r = 0, byte g = 0, byte b = 0, byte a = 255)
		{
			this->r = r;
			this->g = g;
			this->b = b;
			this->a = a;
		}

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

		// Prints the color to the console
		void Print() const
		{
			printf("r: %d g: %d b: %d a: %d\n", r, g, b, a);
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
}
