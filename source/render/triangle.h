#pragma once

#include "math/math.h"
#include "render/vertex.h"


namespace kl
{
	struct triangle
	{
		kl::vertex a, b, c;

		triangle();
		triangle(const kl::vertex& a, const kl::vertex& b, const kl::vertex& c);

		kl::float4 constants() const;
		kl::float3 weights(const kl::float4& consts, const kl::float2& pos) const;
		kl::float3 weights(const kl::float3& point) const;

		bool in(const kl::float3& weights) const;

		float interpolate(const kl::float3& weights, const kl::float3& values) const;
		kl::vertex interpolate(const kl::float3& weights) const;
	};

	// std::cout
	std::ostream& operator<<(std::ostream& os, const kl::triangle& obj);
}