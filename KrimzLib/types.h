#pragma once
#include <iostream>
#include <vector>
#include <windows.h>


namespace kl {
	/* Vectors/Size */
	typedef POINT point;
	struct vec2 {
		double x = 0;
		double y = 0;

		// Prints the vector to the console
		void Print() {
			std::cout << "x: " << x << " y: " << y;
		}

		// Returns the vectors lenght
		double Lenght() {
			return sqrt(x * x + y * y);
		}

		// Normalizes a vector
		void Normalize() {
			double vecLen = this->Lenght();
			x /= vecLen;
			y /= vecLen;
		}

		// Retruns a normalized vector
		vec2 Normalized() {
			double vecLen = this->Lenght();
			return { x / vecLen, y / vecLen };
		}
	};
	struct vec3 {
		double x = 0;
		double y = 0;
		double z = 0;

		// Prints the vector to the console
		void Print() {
			std::cout << "x: " << x << " y: " << y << " z: " << z;
		}

		// Returns the vectors lenght
		double Lenght() {
			return sqrt(x * x + y * y + z * z);
		}

		// Normalizes a vector
		void Normalize() {
			double vecLen = this->Lenght();
			x /= vecLen;
			y /= vecLen;
			z /= vecLen;
		}

		// Retruns a normalized vector
		vec3 Normalized() {
			double vecLen = this->Lenght();
			return { x / vecLen, y / vecLen, z / vecLen };
		}
	};
	struct size {
		int width = 0;
		int height = 0;

		// Returns the area
		int Area() {
			return width * height;
		}
	};

	/* Colors/Bitmaps */
	typedef unsigned char byte;
	struct color {
		byte r = 0;
		byte g = 0;
		byte b = 0;
		byte a = 255;
	};
	struct bitmap {
	public:
		// Constructor
		bitmap(int width, int height) {
			this->width = width;
			this->height = height;
			pixels.resize(size_t(this->width) * size_t(this->height));
		}

		// Getters
		int GetWidth() {
			return this->width;
		}
		int GetHeight() {
			return this->height;
		}
		size_t GetSize() {
			return pixels.size();
		}
		color GetPixel(int x, int y) {
			if (x >= 0 && x < width && y >= 0 && y < height) {
				return pixels[y * size_t(width) + x];
			}
			return { 0, 0, 0, 0 };
		}
		color* GetPixelData() {
			return &this->pixels[0];
		}

		// Setters
		void SetWidth(int width) {
			this->width = width;
			pixels.resize(size_t(this->width) * size_t(this->height));
		}
		void SetHeight(int height) {
			this->height = height;
			pixels.resize(size_t(this->width) * size_t(this->height));
		}
		void SetPixel(int x, int y, color color) {
			if (x >= 0 && x < width && y >= 0 && y < height) {
				pixels[y * size_t(width) + x] = color;
			}
		}

		// Fils the bitmap with solid color
		void FillSolid(color color) {
			std::fill(pixels.begin(), pixels.end(), color);
		}

	private:
		int width = 0;
		int height = 0;
		std::vector<color> pixels;
	};

	/* Files */
	struct filedata {
		std::wstring name;
		std::vector<byte> bytes;
	};

	/* Graphics/OpenGL */
	typedef unsigned int id;
	struct colorf {
		float r = 0;
		float g = 0;
		float b = 0;
		float a = 1;
	};
	struct vertex {
		float x = 0;
		float y = 0;
		float z = 0;
		float u = 0;
		float v = 0;
		colorf color = {};
	};
	struct triangle {
		vertex vertices[3] = {};
	};
}
