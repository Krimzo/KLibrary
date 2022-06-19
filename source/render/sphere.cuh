#pragma once

#include "math/math.cuh"


namespace kl {
	struct sphere {
		kl::float3 center;
		float radius = 0;
		kl::float3 color;
		float reflectivity = 0;
		float emission = 0;

		ALL sphere() {}
		ALL sphere(const kl::float3& center, float radius) : center(center), radius(radius) {}
		ALL sphere(const kl::float3& center, float radius, const kl::float3& color, float reflectivity, float emission) : center(center), radius(radius), color(color), reflectivity(reflectivity), emission(emission) {}

		ALL kl::float3 emiss() const {
			return color * emission;
		}
	};

	// std::cout
	inline std::ostream& operator<<(std::ostream& os, const kl::sphere& obj) {
		os << "{" << obj.center << ", " << obj.radius << "}";
		return os;
	}
}
