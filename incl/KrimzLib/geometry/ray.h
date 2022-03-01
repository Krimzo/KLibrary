#pragma once

#include "KrimzLib/vecmath/vec3.h"
#include "KrimzLib/geometry/plane.h"
#include "KrimzLib/geometry/sphere.h"
#include "KrimzLib/geometry/triangle.h"


namespace kl {
	class ray {
	public:
		kl::vec3 origin;
		kl::vec3 direction;

		// Constructors
		kl::ray();
		kl::ray(const kl::vec3& origin, const kl::vec3& direction);

		// Intersection with a plane
		bool intersect(const kl::plane& plane, kl::vec3* outInter = nullptr) const;

		// Intersection with a triangle
		bool intersect(const kl::triangle& triangle, kl::vec3* outInter = nullptr) const;

		// Intersection with a sphere
		bool intersect(const kl::sphere& sphere, kl::vec3* outInter = nullptr, float* outDis = nullptr) const;
	};
}
