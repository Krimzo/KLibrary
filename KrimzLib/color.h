#pragma once
#include <iostream>
#include <vector>


/* --- TYPES --- */
namespace kl
{
	struct colord
	{
		double r = 0;
		double g = 0;
		double b = 0;
		double a = 1;

		// Prints the color to the console
		void Print() const
		{
			std::cout << "r: " << r << " g: " << g << " b: " << b << " a: " << a << '\n';
		}
	};

	struct color
	{
		byte r = 0;
		byte g = 0;
		byte b = 0;
		byte a = 255;

		// Returns the color as a float type
		colord ToDouble() const
		{
			return { r / 255.0, g / 255.0, b / 255.0, a / 255.0 };
		}

		// Prints the color to the console
		void Print() const
		{
			std::cout << "r: " << r << " g: " << g << " b: " << b << " a: " << a << '\n';
		}
	};

	struct bitmap
	{
	public:
		// Constructor
		bitmap(int width, int height)
		{
			this->width = width;
			this->height = height;
			pixels.resize(size_t(this->width) * size_t(this->height));
		}

		// Getters
		int GetWidth()
		{
			return this->width;
		}
		int GetHeight()
		{
			return this->height;
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
			return { 0, 0, 0, 0 };
		}
		color* GetPixelData()
		{
			return &this->pixels[0];
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
