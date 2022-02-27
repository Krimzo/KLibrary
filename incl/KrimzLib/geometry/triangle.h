#pragma once

#include "KrimzLib/vecmath/vec2.h"
#include "KrimzLib/vecmath/vec3.h"
#include "KrimzLib/vecmath/vec4.h"
#include "KrimzLib/geometry/vertex.h"


namespace kl {
	class triangle {
	public:
		kl::vertex a;
		kl::vertex b;
		kl::vertex c;

		// Constructors
		triangle();
		triangle(const kl::vertex& a, const kl::vertex& b, const kl::vertex& c);

		// Computes and stores the barycentric constants
		kl::vec4 getConsts();

		// Checks if the point is inside the triangle
		bool inTriangle(const kl::vec4& interConsts, const kl::vec2& pos) const;

		// Interpolates and returns the given values
		float interpolate(const kl::vec4& interConsts, const kl::vec3& values, const kl::vec2& pos) const;
		kl::vertex interpolate(const kl::vec4& interConsts, const kl::vec2& pos) const;

	private:
		// Calculates and returns the 3 barycentric weights of a triangle and a point
		kl::vec3 getWeights(const kl::vec4& interConsts, const kl::vec2& pos) const;
	};
}