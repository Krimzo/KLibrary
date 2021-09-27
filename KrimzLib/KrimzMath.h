#pragma once
#include "KrimzLib/KrimzTypes.h"


namespace kl {
	class math {
	public:
		// Returns a dot product of 2 given vectors
		static double VectorDotProd(vec3 a, vec3 b) {
			return a.x * b.x + a.y * b.y + a.z * b.z;
		}
		// Returns a cross product of 2 given vectors
		static vec3 VectorCrossProd(vec3 a, vec3 b) {
			return { a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x };
		}
	};
}
