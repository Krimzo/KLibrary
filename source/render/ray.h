#pragma once

#include "math/math.h"
#include "render/plane.h"
#include "render/sphere.h"
#include "render/triangle.h"
#include "render/camera.h"


namespace kl
{
	struct ray
	{
		kl::float3 origin;
		kl::float3 direction;

		ray();
		ray(const kl::float3& origin, const kl::float3& direction);
		ray(const kl::float3& origin, const kl::mat4& invCamMat, const kl::float2& ndc);
		ray(const kl::camera& cam, const kl::float2& ndc);

		bool intersect(const kl::plane& plane, kl::float3* outInter = nullptr) const;
		bool intersect(const kl::triangle& triangle, kl::float3* outInter = nullptr) const;
		bool intersect(const kl::sphere& sphere, kl::float3& outInter, float& outDis) const;
		bool intersect(const kl::sphere& sphere) const;
	};

	// std::cout
	std::ostream& operator<<(std::ostream& os, const kl::ray& obj);
}
