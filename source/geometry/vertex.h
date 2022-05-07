#pragma once 

#include "math/float3.h"


namespace kl {
	class vertex {
	public:
		kl::float3 world;
		kl::float2 texture;
		kl::float3 normal;

		vertex();
		vertex(const kl::float3& world);
		vertex(const kl::float3& world, const kl::float2& texture);
		vertex(const kl::float3& world, const kl::float3& normal);
		vertex(const kl::float3& world, const kl::float2& texture, const kl::float3& normal);

		// Comparison
		bool equals(const kl::vertex& obj) const;
		bool operator==(const kl::vertex& obj) const;
		bool operator!=(const kl::vertex& obj) const;
	};

	// Overloading std::cout
	std::ostream& operator<<(std::ostream& os, const kl::vertex& obj);
}