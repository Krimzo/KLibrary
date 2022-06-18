#include "render/triangle.h"


kl::triangle::triangle() {}
kl::triangle::triangle(const kl::vertex& a, const kl::vertex& b, const kl::vertex& c) : a(a), b(b), c(c) {}

kl::float4 kl::triangle::constants() const {
	const float tempConst = 1.0f / ((b.world.y - c.world.y) * (a.world.x - c.world.x) + (c.world.x - b.world.x) * (a.world.y - c.world.y));
	return kl::float4(
		(b.world.y - c.world.y) * tempConst,
		(c.world.x - b.world.x) * tempConst,
		(c.world.y - a.world.y) * tempConst,
		(a.world.x - c.world.x) * tempConst
	);
}

kl::float3 kl::triangle::weights(const kl::float4& consts, const kl::float2& pos) const {
	const float dx = pos.x - c.world.x;
	const float dy = pos.y - c.world.y;
	const float interWeight1 = dx * consts.x + dy * consts.y;
	const float interWeight2 = dx * consts.z + dy * consts.w;
	return kl::float3(interWeight1, interWeight2, 1.0f - interWeight1 - interWeight2);
}

bool kl::triangle::in(const kl::float4& consts, const kl::float2& pos) const {
	const kl::float3 w = weights(consts, pos);
	return !(w.x < 0.0f || w.y < 0.0f || w.z < 0.0f);
}

float kl::triangle::interpolate(const kl::float4& consts, const kl::float3& values, const kl::float2& pos) const {
	const kl::float3 w = weights(consts, pos);
	return (values.x * w.x + values.y * w.y + values.z * w.z);
}
kl::vertex kl::triangle::interpolate(const kl::float4& consts, const kl::float2& pos) const {
	const kl::float3 w = weights(consts, pos);
	return {
		kl::float3 {
			a.world.x * w.x + b.world.x * w.y + c.world.x * w.z,
			a.world.y * w.x + b.world.y * w.y + c.world.y * w.z,
			a.world.z * w.x + b.world.z * w.y + c.world.z * w.z
		},
		kl::float2 {
			a.texture.x * w.x + b.texture.x * w.y + c.texture.x * w.z,
			a.texture.y * w.x + b.texture.y * w.y + c.texture.y * w.z
		},
		kl::float3 {
			a.normal.x * w.x + b.normal.x * w.y + c.normal.x * w.z,
			a.normal.y * w.x + b.normal.y * w.y + c.normal.y * w.z,
			a.normal.z * w.x + b.normal.z * w.y + c.normal.z * w.z
		}
	};
}
kl::vertex kl::triangle::interpolate(const kl::float3& point) const {
	const kl::float3 v0 = a.world - c.world;
	const kl::float3 v1 = b.world - c.world;
	const kl::float3 v2 = point - c.world;
	const float d00 = v0.dot(v0);
	const float d01 = v0.dot(v1);
	const float d11 = v1.dot(v1);
	const float d20 = v2.dot(v0);
	const float d21 = v2.dot(v1);
	const float invDenom = 1.0f / (d00 * d11 - d01 * d01);
	const float w1 = (d11 * d20 - d01 * d21) * invDenom;
	const float w2 = (d00 * d21 - d01 * d20) * invDenom;
	const float w3 = 1.0f - w1 - w2;
	return {
		point,
		a.texture * w1 + b.texture * w2 + c.texture * w3,
		a.normal * w1 + b.normal * w2 + c.normal * w3
	};
}

// std::cout
std::ostream& kl::operator<<(std::ostream& os, const kl::triangle& obj) {
	os << "{" << obj.a << ", " << obj.b << ", " << obj.c << "}";
	return os;
}
