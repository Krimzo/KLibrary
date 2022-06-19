#pragma once

#include "math/math.cuh"


namespace kl {
	struct plane {
		kl::float3 normal;
		kl::float3 point;

		ALL plane() {}
		ALL plane(const kl::float3& normal, const kl::float3& point) : normal(normal.norm()), point(point) {}
	};

	// std::cout
	inline std::ostream& operator<<(std::ostream& os, const kl::plane& obj) {
		os << "{" << obj.normal << ", " << obj.point << "}";
		return os;
	}
}
