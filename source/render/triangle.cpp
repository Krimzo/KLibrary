#include "render/triangle.h"


kl::Triangle::Triangle() {}

kl::Triangle::Triangle(const kl::Vertex& a, const kl::Vertex& b, const kl::Vertex& c) : a(a), b(b), c(c) {}

kl::Float4 kl::Triangle::getConstants() const {
	const float tempConst = 1.0f / ((b.world.y - c.world.y) * (a.world.x - c.world.x) + (c.world.x - b.world.x) * (a.world.y - c.world.y));
	return {
		(b.world.y - c.world.y) * tempConst,
		(c.world.x - b.world.x) * tempConst,
		(c.world.y - a.world.y) * tempConst,
		(a.world.x - c.world.x) * tempConst
	};
}

kl::Float3 kl::Triangle::getWeights(const Float4& constants, const Float2& point) const {
	const float dx = point.x - c.world.x;
	const float dy = point.y - c.world.y;
	const float w1 = dx * constants.x + dy * constants.y;
	const float w2 = dx * constants.z + dy * constants.w;
	return { w1, w2, 1.0f - w1 - w2 };
}

kl::Float3 kl::Triangle::getWeights(const kl::Float3& position) const {
	const kl::Float3 v0 = a.world - c.world;
	const kl::Float3 v1 = b.world - c.world;
	const kl::Float3 v2 = position - c.world;
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

bool kl::Triangle::isInTriangle(const Float3& weights) const {
	return !(weights.x < 0.0f || weights.y < 0.0f || weights.z < 0.0f);
}

float kl::Triangle::interpolate(const Float3& weights, const Float3& values) const {
	return weights.dot(values);
}

kl::Vertex kl::Triangle::interpolate(const kl::Float3& weights) const {
	return {
		Float3{
			interpolate(weights, { a.world.x, b.world.x, c.world.x }),
			interpolate(weights, { a.world.y, b.world.y, c.world.y }),
			interpolate(weights, { a.world.z, b.world.z, c.world.z })
	},
		Float2{
			interpolate(weights, { a.texture.x, b.texture.x, c.texture.x }),
			interpolate(weights, { a.texture.y, b.texture.y, c.texture.y })
	},
		Float3{
			interpolate(weights, { a.normal.x, b.normal.x, c.normal.x }),
			interpolate(weights, { a.normal.y, b.normal.y, c.normal.y }),
			interpolate(weights, { a.normal.z, b.normal.z, c.normal.z })
	}
	};
}

// std::cout
std::ostream& kl::operator<<(std::ostream& os, const kl::Triangle& obj) {
	os << "{" << obj.a << ", " << obj.b << ", " << obj.c << "}";
	return os;
}
