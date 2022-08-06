#pragma once

#include "math/math.h"
#include "render/plane.h"
#include "render/sphere.h"
#include "render/triangle.h"
#include "render/camera.h"


namespace kl {
	struct Ray {
		kl::Float3 origin;
		kl::Float3 direction;

		Ray();
		Ray(const kl::Float3& origin, const kl::Float3& direction);
		Ray(const kl::Float3& origin, const Mat4& inverseMatrix, const Float2& NDC);
		Ray(const Camera& camera, const Float2& NDC);

		bool intersectPlane(const kl::Plane& plane, kl::Float3* outIntersection = nullptr) const;

		bool intersectTriangle(const kl::Triangle& triangle, kl::Float3* outIntersection = nullptr) const;

		bool intersectSphere(const Sphere& sphere, Float3* outIntersection = nullptr, float* outDistance = nullptr) const;
		bool intersectSphere(const Sphere& sphere) const;
	};

	// std::cout
	std::ostream& operator<<(std::ostream& os, const kl::Ray& obj);
}
