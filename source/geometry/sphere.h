#pragma once

#include "math/float3.h"


namespace kl
{
	class sphere
	{
	public:
		// Geometry
		kl::float3 center;
		float radius = 0;

		// Light
		kl::float3 color;
		float reflectivity = 0;
		float emission = 0;

		// Constructor
		sphere();
		sphere(const kl::float3& center, float radius, const kl::float3& color, float reflectivity = 0, float emission = 0);

		// Calculates the emissive color
		kl::float3 calcEmiss() const;
	};
}
