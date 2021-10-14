#pragma once
#include <windows.h>
#include "KrimzLib/types.h"
#include "KrimzLib/graphics.h"


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
