#pragma once

#include "math/math.h"
#include "render/plane.h"
#include "render/sphere.h"
#include "render/triangle.h"


namespace kl {
	class ray {
	public:
		kl::float3 origin;
		kl::float3 direction;

		kl::ray();
		kl::ray(const kl::float3& origin, const kl::float3& direction);

		// Intersection with a plane
		bool intersect(const kl::plane& plane, kl::float3* outInter = nullptr) const;

		// Intersection with a triangle
		bool intersect(const kl::triangle& triangle, kl::float3* outInter = nullptr) const;

		// Intersection with a sphere
		bool intersect(const kl::sphere& sphere, kl::float3* outInter = nullptr, float* outDis = nullptr) const;
	};

	// std::cout
	std::ostream& operator<<(std::ostream& os, const kl::ray& obj);
}
