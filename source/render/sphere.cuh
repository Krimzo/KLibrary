#pragma once

#include "math/math.cuh"


namespace kl {
	class sphere {
	public:
		kl::float3 center;
		float radius = 0;
		kl::float3 color;
		float reflectivity = 0;
		float emission = 0;

		ALL sphere();
		ALL sphere(const kl::float3& center, float radius, const kl::float3& color, float reflectivity, float emission);

		ALL kl::float3 emiss() const;
	};

	// std::cout
	std::ostream& operator<<(std::ostream& os, const kl::sphere& obj);
}
