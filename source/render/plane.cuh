#pragma once

#include "math/math.cuh"


namespace kl {
	class plane {
	public:
		kl::float3 normal;
		kl::float3 point;

		ALL plane();
		ALL plane(const kl::float3& normal, const kl::float3& point);
	};

	// std::cout
	std::ostream& operator<<(std::ostream& os, const kl::plane& obj);
}
