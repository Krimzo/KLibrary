#pragma once
#include <windows.h>
#include "KrimzLib/color.h"


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

	struct vertex
	{
		double x = 0;
		double y = 0;
		double z = 0;
		double u = 0;
		double v = 0;
		colord color = {};

		// Resizes the vertex
		void Resize(vec3 size)
		{
			x *= size.x;
			y *= size.y;
			z *= size.z;
		}

		// Rotates the vertex
		void Rotate(vec3 rotation)
		{
			// Transforming degrees to radians
			rotation.x *= constant::toRadians;
			rotation.y *= constant::toRadians;
			rotation.z *= constant::toRadians;

			// Calculating trig function only once to gain performance
			double sinx = sin(rotation.x);
			double cosx = cos(rotation.x);
			double siny = sin(rotation.y);
			double cosy = cos(rotation.y);
			double sinz = sin(rotation.z);
			double cosz = cos(rotation.z);

			// Rotating
			vec3 tempPoint;
			tempPoint.x = x * (cosz * cosy) + y * (cosz * siny * sinx - sinz * cosx) + z * (cosz * siny * cosx + sinz * sinx);
			tempPoint.y = x * (sinz * cosy) + y * (sinz * siny * sinx + cosz * cosx) + z * (sinz * siny * cosx - cosz * sinx);
			tempPoint.z = x * (-siny) + y * (cosy * sinx) + z * (cosy * cosx);
			x = tempPoint.x;
			y = tempPoint.y;
			z = tempPoint.z;
		}

		// Translates the vertex
		void Translate(vec3 translation)
		{
			x += translation.x;
			y += translation.y;
			z += translation.z;
		}
	};

	struct triangle
	{
		vertex vertices[3] = {};
		bool textured = false;
		unsigned int texture = 0;

		// Returns true of false respectively if the point is inside the triangle
		bool ContainsPoint(point pt)
		{
			double d1 = (pt.x - vertices[1].x) * (vertices[0].y - vertices[1].y) - (vertices[0].x - vertices[1].x) * (pt.y - vertices[1].y);
			double d2 = (pt.x - vertices[2].x) * (vertices[1].y - vertices[2].y) - (vertices[1].x - vertices[2].x) * (pt.y - vertices[2].y);
			double d3 = (pt.x - vertices[0].x) * (vertices[2].y - vertices[0].y) - (vertices[2].x - vertices[0].x) * (pt.y - vertices[0].y);
			return !((d1 < 0 || d2 < 0 || d3 < 0) && (d1 > 0 || d2 > 0 || d3 > 0));
		}
		bool ContainsPoint(int x, int y)
		{
			double d1 = (x - vertices[1].x) * (vertices[0].y - vertices[1].y) - (vertices[0].x - vertices[1].x) * (y - vertices[1].y);
			double d2 = (x - vertices[2].x) * (vertices[1].y - vertices[2].y) - (vertices[1].x - vertices[2].x) * (y - vertices[2].y);
			double d3 = (x - vertices[0].x) * (vertices[2].y - vertices[0].y) - (vertices[2].x - vertices[0].x) * (y - vertices[0].y);
			return !((d1 < 0 || d2 < 0 || d3 < 0) && (d1 > 0 || d2 > 0 || d3 > 0));
		}

		// Calculates and returns 3 triangle barycentric ratios
		vec3 CalculateBarycentricRatios(int x, int y)
		{
			double baryRatio1 = ((vertices[1].y - vertices[2].y) * (x - vertices[2].x) + (vertices[2].x - vertices[1].x) * (y - vertices[2].y)) / ((vertices[1].y - vertices[2].y) * (vertices[0].x - vertices[2].x) + (vertices[2].x - vertices[1].x) * (vertices[0].y - vertices[2].y));
			double baryRatio2 = ((vertices[2].y - vertices[0].y) * (x - vertices[2].x) + (vertices[0].x - vertices[2].x) * (y - vertices[2].y)) / ((vertices[1].y - vertices[2].y) * (vertices[0].x - vertices[2].x) + (vertices[2].x - vertices[1].x) * (vertices[0].y - vertices[2].y));
			return { baryRatio1, baryRatio2,  1 - baryRatio1 - baryRatio2 };
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
		static double VectorDotProd(vec2 a, vec2 b)
		{
			return a.x * b.x + a.y * b.y;
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
	};
}
