#include "render/triangle.h"


kl::triangle::triangle() {}
kl::triangle::triangle(const kl::vertex& a, const kl::vertex& b, const kl::vertex& c) : a(a), b(b), c(c) {}

kl::float4 kl::triangle::constants() const {
	const float tempConst = 1.0f / ((b.world.y - c.world.y) * (a.world.x - c.world.x) + (c.world.x - b.world.x) * (a.world.y - c.world.y));
	return {
		(b.world.y - c.world.y) * tempConst,
		(c.world.x - b.world.x) * tempConst,
		(c.world.y - a.world.y) * tempConst,
		(a.world.x - c.world.x) * tempConst
	};
}
kl::float3 kl::triangle::weights(const kl::float4& consts, const kl::float2& pos) const {
	const float dx = pos.x - c.world.x;
	const float dy = pos.y - c.world.y;
	const float w1 = dx * consts.x + dy * consts.y;
	const float w2 = dx * consts.z + dy * consts.w;
	return { w1, w2, 1.0f - w1 - w2 };
}
kl::float3 kl::triangle::weights(const kl::float3& point) const {
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
	return { w1, w2, 1.0f - w1 - w2 };
}

bool kl::triangle::in(const kl::float3& weights) const {
	return !(weights.x < 0.0f || weights.y < 0.0f || weights.z < 0.0f);
}

float kl::triangle::interpolate(const kl::float3& weights, const kl::float3& values) const {
	return weights.dot(values);
}
kl::vertex kl::triangle::interpolate(const kl::float3& weights) const {
	return {
		kl::float3 {
			interpolate(weights, { a.world.x, b.world.x, c.world.x }),
			interpolate(weights, { a.world.y, b.world.y, c.world.y }),
			interpolate(weights, { a.world.z, b.world.z, c.world.z })
		},
		kl::float2 {
			interpolate(weights, { a.texture.x, b.texture.x, c.texture.x }),
			interpolate(weights, { a.texture.y, b.texture.y, c.texture.y })
		},
		kl::float3 {
			interpolate(weights, { a.normal.x, b.normal.x, c.normal.x }),
			interpolate(weights, { a.normal.y, b.normal.y, c.normal.y }),
			interpolate(weights, { a.normal.z, b.normal.z, c.normal.z })
		}
	};
}

// std::cout
std::ostream& kl::operator<<(std::ostream& os, const kl::triangle& obj) {
	os << "{" << obj.a << ", " << obj.b << ", " << obj.c << "}";
	return os;
}
