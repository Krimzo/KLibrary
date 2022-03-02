#pragma once

#include "KrimzLib/math/float3.h"
#include "KrimzLib/geometry/plane.h"
#include "KrimzLib/geometry/sphere.h"
#include "KrimzLib/geometry/triangle.h"


namespace kl {
	class ray {
	public:
		kl::float3 origin;
		kl::float3 direction;

		// Constructors
		kl::ray();
		kl::ray(const kl::float3& origin, const kl::float3& direction);

		// Intersection with a plane
		bool intersect(const kl::plane& plane, kl::float3* outInter = nullptr) const;

		// Intersection with a triangle
		bool intersect(const kl::triangle& triangle, kl::float3* outInter = nullptr) const;

		// Intersection with a sphere
		bool intersect(const kl::sphere& sphere, kl::float3* outInter = nullptr, float* outDis = nullptr) const;
	};
}
