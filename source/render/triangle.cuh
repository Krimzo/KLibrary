#pragma once

#include "math/math.cuh"
#include "render/vertex.cuh"


namespace kl {
	class triangle {
	private:
		ALL kl::float3 weights(const kl::float4& interConsts, const kl::float2& pos) const;

	public:
		kl::vertex a, b, c;

		ALL triangle();
		ALL triangle(const kl::vertex& a, const kl::vertex& b, const kl::vertex& c);

		ALL kl::float4 constants() const;

		ALL bool in(const kl::float4& consts, const kl::float2& pos) const;

		ALL float interpolate(const kl::float4& consts, const kl::float3& values, const kl::float2& pos) const;
		ALL kl::vertex interpolate(const kl::float4& consts, const kl::float2& pos) const;
		ALL kl::vertex interpolate(const kl::float3& point) const;
	};

	// std::cout
	std::ostream& operator<<(std::ostream& os, const kl::triangle& obj);
}