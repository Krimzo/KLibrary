#include "geometry/triangle.h"


// Constructors
kl::triangle::triangle()
{

}
kl::triangle::triangle(const kl::vertex& a, const kl::vertex& b, const kl::vertex& c)
{
	this->a = a;
	this->b = b;
	this->c = c;
}

// Computes and stores the barycentric constants
kl::float4 kl::triangle::getConsts()
{
	const float tempConst = 1.0f / ((b.world.y - c.world.y) * (a.world.x - c.world.x) + (c.world.x - b.world.x) * (a.world.y - c.world.y));
	return kl::float4(
		(b.world.y - c.world.y) * tempConst,
		(c.world.x - b.world.x) * tempConst,
		(c.world.y - a.world.y) * tempConst,
		(a.world.x - c.world.x) * tempConst
	);
}

// Calculates and returns the 3 barycentric weights of a triangle and a point
kl::float3 kl::triangle::getWeights(const kl::float4& interConsts, const kl::float2& pos) const
{
	const float dx = pos.x - c.world.x;
	const float dy = pos.y - c.world.y;
	const float interWeight1 = dx * interConsts.x + dy * interConsts.y;
	const float interWeight2 = dx * interConsts.z + dy * interConsts.w;
	return kl::float3(interWeight1, interWeight2, 1 - interWeight1 - interWeight2);
}

// Checks if the point is inside the triangle
bool kl::triangle::inTriangle(const kl::float4& interConsts, const kl::float2& pos) const
{
	const kl::float3 weights = getWeights(interConsts, pos);
	return !(weights.x < 0 || weights.y < 0 || weights.z < 0);
}

// Interpolates and returns the depth
float kl::triangle::interpolate(const kl::float4& interConsts, const kl::float3& values, const kl::float2& pos) const
{
	const kl::float3 weights = getWeights(interConsts, pos);
	return values.x * weights.x + values.y * weights.y + values.z * weights.z;
}
kl::vertex kl::triangle::interpolate(const kl::float4& interConsts, const kl::float2& pos) const
{
	const kl::float3 weights = getWeights(interConsts, pos);
	return kl::vertex(
		kl::float3(
			a.world.x * weights.x + b.world.x * weights.y + c.world.x * weights.z,
			a.world.y * weights.x + b.world.y * weights.y + c.world.y * weights.z,
			a.world.z * weights.x + b.world.z * weights.y + c.world.z * weights.z
		),
		kl::float2(
			a.texture.x * weights.x + b.texture.x * weights.y + c.texture.x * weights.z,
			a.texture.y * weights.x + b.texture.y * weights.y + c.texture.y * weights.z
		),
		kl::float3(
			a.normal.x * weights.x + b.normal.x * weights.y + c.normal.x * weights.z,
			a.normal.y * weights.x + b.normal.y * weights.y + c.normal.y * weights.z,
			a.normal.z * weights.x + b.normal.z * weights.y + c.normal.z * weights.z
		)
	);
}
