#pragma once

#include "Math/math.h"
#include "Render/Plane.h"
#include "Render/Sphere.h"
#include "Render/Triangle.h"
#include "Render/Camera.h"


namespace kl {
	struct Ray {
		Float3 origin;
		Float3 direction;

		Ray();
		Ray(const Float3& origin, const Float3& direction);
		Ray(const Float3& origin, const Mat4& inverseMatrix, const Float2& NDC);
		Ray(const Camera& camera, const Float2& NDC);

		bool intersectPlane(const Plane& plane, Float3* outIntersection = nullptr) const;

		bool intersectTriangle(const Triangle& triangle, Float3* outIntersection = nullptr) const;

		bool intersectSphere(const Sphere& sphere, Float3* outIntersection = nullptr, float* outDistance = nullptr) const;
		bool intersectSphere(const Sphere& sphere) const;
	};

	// std::cout
	std::ostream& operator<<(std::ostream& os, const Ray& obj);
}
