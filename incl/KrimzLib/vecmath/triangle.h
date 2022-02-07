#pragma once

#include "KrimzLib/vecmath/vec2.h"
#include "KrimzLib/vecmath/vec3.h"
#include "KrimzLib/vecmath/vec4.h"


namespace kl {
	class triangle {
	public:
		kl::vec2 a;
		kl::vec2 b;
		kl::vec2 c;

		// Constructors
		triangle();
		triangle(const kl::vec2& a, const kl::vec2& b, const kl::vec2& c);

		// Computes and stores the barycentric constants
		void calcConsts();

		// Checks if the point is inside the triangle
		bool inTriangle(const kl::vec2& pos) const;

		// Interpolates and returns the depth
		float interpolate(const kl::vec3& values, const kl::vec2& pos) const;

	private:
		// Interpolation constants
		kl::vec4 interConsts;

		// Calculates and returns the 3 barycentric weights of a triangle and a point
		kl::vec3 getWeights(const kl::vec2& pos) const;
	};
}