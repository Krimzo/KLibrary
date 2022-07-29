#pragma once

#include "math/math.h"


namespace kl
{
	struct sphere
	{
		kl::float3 center;
		float radius = 0;

		kl::float3 color;
		float reflectivity = 0;
		float emission = 0;

		sphere();
		sphere(const kl::float3& center, float radius);
		sphere(const kl::float3& center, float radius, const kl::float3& color, float reflectivity, float emission);

		kl::float3 emissive() const;
	};

	// std::cout
	std::ostream& operator<<(std::ostream& os, const kl::sphere& obj);
}
