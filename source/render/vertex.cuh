#pragma once 

#include "math/math.cuh"


namespace kl {
	struct vertex {
		kl::float3 world;
		kl::float2 texture;
		kl::float3 normal;

		ALL vertex();
		ALL vertex(const kl::float3& world);
		ALL vertex(const kl::float3& world, const kl::float2& texture);
		ALL vertex(const kl::float3& world, const kl::float3& normal);
		ALL vertex(const kl::float3& world, const kl::float2& texture, const kl::float3& normal);
	};

	// std::cout
	std::ostream& operator<<(std::ostream& os, const kl::vertex& obj);
}