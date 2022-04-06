#pragma once

#include "math/float2.h"
#include "math/float3.h"
#include "math/float4.h"
#include "geometry/vertex.h"


namespace kl
{
	class triangle
	{
	public:
		kl::vertex a;
		kl::vertex b;
		kl::vertex c;

		// Constructors
		triangle();
		triangle(const kl::vertex& a, const kl::vertex& b, const kl::vertex& c);

		// Computes and stores the barycentric constants
		kl::float4 getConsts();

		// Checks if the point is inside the triangle
		bool inTriangle(const kl::float4& interConsts, const kl::float2& pos) const;

		// Interpolates and returns the given values
		float interpolate(const kl::float4& interConsts, const kl::float3& values, const kl::float2& pos) const;
		kl::vertex interpolate(const kl::float4& interConsts, const kl::float2& pos) const;

	private:
		// Calculates and returns the 3 barycentric weights of a triangle and a point
		kl::float3 getWeights(const kl::float4& interConsts, const kl::float2& pos) const;
	};
}