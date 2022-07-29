#pragma once 

#include "math/math.h"


namespace kl
{
	struct vertex
	{
		kl::float3 world;
		kl::float2 texture;
		kl::float3 normal;

		vertex();
		vertex(const kl::float3& world);
		vertex(const kl::float3& world, const kl::float2& texture);
		vertex(const kl::float3& world, const kl::float3& normal);
		vertex(const kl::float3& world, const kl::float2& texture, const kl::float3& normal);
	};

	// std::cout
	std::ostream& operator<<(std::ostream& os, const kl::vertex& obj);
}