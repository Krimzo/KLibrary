#pragma once
#include <iostream>
#include <vector>
#include <windows.h>
#include "KrimzLib/incl/OpenCL/cl.hpp"


/* MATH */
namespace kl
{
	typedef POINT point;
	struct vec2
	{
		double x = 0;
		double y = 0;

		// Operator overloading
		vec2 operator + (vec2 obj)
		{
			return { x + obj.x, y + obj.y };
		}
		vec2 operator - (vec2 obj)
		{
			return { x - obj.x, y - obj.y };
		}
		vec2 operator * (int obj)
		{
			return { x * obj, y * obj };
		}
		vec2 operator * (double obj)
		{
			return { x * obj, y * obj };
		}
		vec2 operator / (int obj)
		{
			return { x / obj, y / obj };
		}
		vec2 operator / (double obj)
		{
			return { x / obj, y / obj };
		}
		bool operator == (vec2 obj)
		{
			return (x == obj.x && y == obj.y);
		}
		bool operator != (vec2 obj)
		{
			return (x != obj.x || y != obj.y);
		}

		// Returns the vectors lenght
		double Lenght()
		{
			return sqrt(x * x + y * y);
		}

		// Normalizes a vector
		void Normalize()
		{
			double vecLen = this->Lenght();
			x /= vecLen;
			y /= vecLen;
		}

		// Retruns a normalized vector
		vec2 Normalized()
		{
			double vecLen = this->Lenght();
			return { x / vecLen, y / vecLen };
		}
	};

	struct vec3
	{
		double x = 0;
		double y = 0;
		double z = 0;

		// Operator overloading
		vec3 operator + (vec3 obj)
		{
			return { x + obj.x, y + obj.y, z + obj.z };
		}
		vec3 operator - (vec3 obj)
		{
			return { x - obj.x, y - obj.y, z - obj.z };
		}
		vec3 operator * (int obj)
		{
			return { x * obj, y * obj, z * obj };
		}
		vec3 operator * (double obj)
		{
			return { x * obj, y * obj, z * obj };
		}
		vec3 operator / (int obj)
		{
			return { x / obj, y / obj, z / obj };
		}
		vec3 operator / (double obj)
		{
			return { x / obj, y / obj, z / obj };
		}
		bool operator == (vec3 obj)
		{
			return (x == obj.x && y == obj.y && z == obj.z);
		}
		bool operator != (vec3 obj)
		{
			return (x != obj.x || y != obj.y || z != obj.z);
		}

		// Returns the vectors lenght
		double Lenght()
		{
			return sqrt(x * x + y * y + z * z);
		}

		// Normalizes a vector
		void Normalize()
		{
			double vecLen = this->Lenght();
			x /= vecLen;
			y /= vecLen;
			z /= vecLen;
		}

		// Retruns a normalized vector
		vec3 Normalized()
		{
			double vecLen = this->Lenght();
			return { x / vecLen, y / vecLen, z / vecLen };
		}
	};

	struct size
	{
		int width = 0;
		int height = 0;
	};
}

/* COLOR */
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

		// Converts a bitmap to an ASCII frame
		std::string ToASCIIFrame(size frameSize)
		{
			// Calculations
			int pixelWidthIncrement = width / frameSize.width;
			int pixelHeightIncrement = height / frameSize.height;

			// Processing
			std::string frame = "";
			for (int y = 0; y < frameSize.height; y++)
			{
				for (int x = 0; x < frameSize.width; x++)
				{
					// Pixels to grayscale
					color currentPixel = this->GetPixel(x * pixelWidthIncrement, y * pixelHeightIncrement);
					int grayScaledPixel = (int)(currentPixel.r * 0.299 + currentPixel.g * 0.587 + currentPixel.b * 0.114);

					// Grayscaled values to ASCII
					int saturationLevel = (int)((grayScaledPixel / 255.0) * 9);
					frame += asciiPixelTable[saturationLevel];
				}
				frame += '\n';
			}
			return frame;
		}

	private:
		int width = 0;
		int height = 0;
		std::vector<color> pixels = {};
		char asciiPixelTable[10] = { '@', '%', '#', 'x', '+', '=', ':', '-', '.', ' ' };
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

/* BINARY */
namespace kl
{
	typedef std::vector<byte> bytes;
	typedef std::string bits;
}

/* OPENCL */
namespace kl
{
	typedef void* cpumem;
	typedef cl_mem gpumem;
	typedef cl_program clprogram;
	typedef cl_kernel clkernel;
}

/* OPENGL */
namespace kl
{
	typedef unsigned int texture;
}

/* ENGINE */
namespace kl
{
	struct vertex
	{
		double x = 0;
		double y = 0;
		double z = 0;
		double u = 0;
		double v = 0;
		colord color = {};
	};

	struct triangle
	{
		vertex vertices[3] = {};
		bool textured = false;
	};
	
	struct gameobject
	{
		// Looks
		bool visible = true;
		texture texture = 0;
		std::vector<triangle> triangles = {};
		
		// Geometry
		vec3 position = {};
		vec3 rotation = {};
		vec3 size = { 1, 1, 1 };

		// Physics
		bool physics = false;
		double gravityMulti = 1;
		vec3 velocity = {};
		vec3 angularMo = {};
	};

	struct camera
	{
		vec3 position = {};
		vec3 rotation = {};
	};
}
