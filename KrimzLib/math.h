#pragma once
#include <windows.h>
#include "KrimzLib/engine.h"


/* --- TYPES --- */
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

		// Prints the vector to the console
		void Print()
		{
			std::cout << "x: " << x << " y: " << y << '\n';
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

		// Prints the vector to the console
		void Print()
		{
			std::cout << "x: " << x << " y: " << y << " z: " << z << '\n';
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

		// Returns the area
		int Area()
		{
			return width * height;
		}
	};
}


/* --- CLASS --- */
namespace kl
{
	class math
	{
	public:
		// Returns a dot product of 2 given vectors
		static double VectorDotProd(vec3 a, vec3 b)
		{
			return a.x * b.x + a.y * b.y + a.z * b.z;
		}

		// Returns a cross product of 2 given vectors
		static vec3 VectorCrossProd(vec3 a, vec3 b)
		{
			return { a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x };
		}

		// Checks if the given number is prime
		static bool IsPrime(size_t num)
		{
			if (num == 0 || num == 1)
				return false;
			
			size_t half = num / 2;
			for (size_t i = 2; i <= half; i++)
			{
				if (num % i == 0)
					return false;
			}
			return true;
		}

		// Calculates and returns 3 triangle barycentric ratios
		static vec3 CalculateBarycentricRatios(triangle& tr, int x, int y)
		{
			double baryRatio1 = ((tr.vertices[1].y - tr.vertices[2].y) * (x - tr.vertices[2].x) + (tr.vertices[2].x - tr.vertices[1].x) * (y - tr.vertices[2].y)) / ((tr.vertices[1].y - tr.vertices[2].y) * (tr.vertices[0].x - tr.vertices[2].x) + (tr.vertices[2].x - tr.vertices[1].x) * (tr.vertices[0].y - tr.vertices[2].y));
			double baryRatio2 = ((tr.vertices[2].y - tr.vertices[0].y) * (x - tr.vertices[2].x) + (tr.vertices[0].x - tr.vertices[2].x) * (y - tr.vertices[2].y)) / ((tr.vertices[1].y - tr.vertices[2].y) * (tr.vertices[0].x - tr.vertices[2].x) + (tr.vertices[2].x - tr.vertices[1].x) * (tr.vertices[0].y - tr.vertices[2].y));
			return { baryRatio1, baryRatio2,  1 - baryRatio1 - baryRatio2 };
		}
		static vec3 CalculateBarycentricRatios(triangle&& tr, int x, int y)
		{
			double baryRatio1 = ((tr.vertices[1].y - tr.vertices[2].y) * (x - tr.vertices[2].x) + (tr.vertices[2].x - tr.vertices[1].x) * (y - tr.vertices[2].y)) / ((tr.vertices[1].y - tr.vertices[2].y) * (tr.vertices[0].x - tr.vertices[2].x) + (tr.vertices[2].x - tr.vertices[1].x) * (tr.vertices[0].y - tr.vertices[2].y));
			double baryRatio2 = ((tr.vertices[2].y - tr.vertices[0].y) * (x - tr.vertices[2].x) + (tr.vertices[0].x - tr.vertices[2].x) * (y - tr.vertices[2].y)) / ((tr.vertices[1].y - tr.vertices[2].y) * (tr.vertices[0].x - tr.vertices[2].x) + (tr.vertices[2].x - tr.vertices[1].x) * (tr.vertices[0].y - tr.vertices[2].y));
			return { baryRatio1, baryRatio2,  1 - baryRatio1 - baryRatio2 };
		}
	};
}
