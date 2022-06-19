#pragma once 

#include "math/math.cuh"


namespace kl {
	struct vertex {
		kl::float3 world;
		kl::float2 texture;
		kl::float3 normal;

		ALL vertex() {}
		ALL vertex(const kl::float3& world) : world(world) {}
		ALL vertex(const kl::float3& world, const kl::float2& texture) : world(world), texture(texture) {}
		ALL vertex(const kl::float3& world, const kl::float3& normal) : world(world), normal(normal) {}
		ALL vertex(const kl::float3& world, const kl::float2& texture, const kl::float3& normal) : world(world), texture(texture), normal(normal) {}
	};

	// std::cout
	inline std::ostream& operator<<(std::ostream& os, const kl::vertex& obj) {
		os << "{" << obj.world << ", " << obj.texture << ", " << obj.normal << "}";
		return os;
	}
}