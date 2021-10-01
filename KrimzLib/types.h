#pragma once
#include <iostream>
#include <vector>
#include <windows.h>
#include "KrimzLib/incl/OpenCL/cl.hpp"


namespace kl {
	/* ----- Vectors/Size ----- */
	typedef POINT point;

	struct vec2 {
		double x = 0;
		double y = 0;

		// Operator overloading
		vec2 operator + (vec2 obj) {
			return { x + obj.x, y + obj.y };
		}
		vec2 operator - (vec2 obj) {
			return { x - obj.x, y - obj.y };
		}
		vec2 operator * (int obj) {
			return { x * obj, y * obj };
		}
		vec2 operator * (double obj) {
			return { x * obj, y * obj };
		}
		vec2 operator / (int obj) {
			return { x / obj, y / obj };
		}
		vec2 operator / (double obj) {
			return { x / obj, y / obj };
		}
		bool operator == (vec2 obj) {
			return (x == obj.x && y == obj.y);
		}
		bool operator != (vec2 obj) {
			return (x != obj.x || y != obj.y);
		}

		// Prints the vector to the console
		void Print() {
			std::cout << "x: " << x << " y: " << y << '\n';
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

		// Operator overloading
		vec3 operator + (vec3 obj) {
			return { x + obj.x, y + obj.y, z + obj.z };
		}
		vec3 operator - (vec3 obj) {
			return { x - obj.x, y - obj.y, z - obj.z };
		}
		vec3 operator * (int obj) {
			return { x * obj, y * obj, z * obj };
		}
		vec3 operator * (double obj) {
			return { x * obj, y * obj, z * obj };
		}
		vec3 operator / (int obj) {
			return { x / obj, y / obj, z / obj };
		}
		vec3 operator / (double obj) {
			return { x / obj, y / obj, z / obj };
		}
		bool operator == (vec3 obj) {
			return (x == obj.x && y == obj.y && z == obj.z);
		}
		bool operator != (vec3 obj) {
			return (x != obj.x || y != obj.y || z != obj.z);
		}

		// Prints the vector to the console
		void Print() {
			std::cout << "x: " << x << " y: " << y << " z: " << z << '\n';
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


	/* ----- Colors/Bitmaps ----- */
	typedef unsigned char byte;

	struct color {
		byte r = 0;
		byte g = 0;
		byte b = 0;
		byte a = 255;

		// Prints the color to the console
		void Print() {
			std::cout << "r: " << r << " g: " << g << " b: " << b << " a: " << a << '\n';
		}
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


	/* ----- Files ----- */
	struct filedata {
		std::wstring name;
		std::vector<byte> bytes;
	};


	/* ----- OpenGL/OpenCL ----- */
	typedef unsigned int id, index;
	typedef void* cpumem;
	typedef cl_mem gpumem;
	typedef cl_program clprogram;
	typedef cl_kernel clkernel;

	struct colorf {
		float r = 0;
		float g = 0;
		float b = 0;
		float a = 1;

		// Returns a byte type color
		color ToByte() {
			return { byte(r * 255), byte(g * 255), byte(b * 255), byte(a * 255) };
		}

		// Prints the color to the console
		void Print() {
			std::cout << "r: " << r << " g: " << g << " b: " << b << " a: " << a << '\n';
		}
	};

	enum class shadertype {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	struct shaderpackage {
		std::string vertex;
		std::string fragment;
	};


	/* ----- Game Engine ----- */
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
