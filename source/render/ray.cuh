#pragma once

#include "math/math.cuh"
#include "render/plane.cuh"
#include "render/sphere.cuh"
#include "render/triangle.cuh"
#include "render/camera.cuh"


namespace kl {
	class ray {
	public:
		kl::float3 origin;
		kl::float3 direction;

		ALL ray();
		ALL ray(const kl::float3& origin, const kl::float3& direction);
		ALL ray(const kl::float3& origin, const kl::mat4& invCamMat, const kl::float2& ndc);
		ALL ray(const kl::camera& cam, const kl::float2& ndc);

		ALL bool intersect(const kl::plane& plane, kl::float3* outInter = nullptr) const;
		ALL bool intersect(const kl::triangle& triangle, kl::float3* outInter = nullptr) const;
		ALL bool intersect(const kl::sphere& sphere, kl::float3* outInter = nullptr, float* outDis = nullptr) const;
	};

	// std::cout
	std::ostream& operator<<(std::ostream& os, const kl::ray& obj);
}
