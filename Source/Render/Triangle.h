#pragma once

#include "Math/Math.h"
#include "Render/Vertex.h"


namespace kl {
	struct Triangle {
		Vertex a, b, c;

		Triangle();
		Triangle(const Vertex& a, const Vertex& b, const Vertex& c);

		Float4 getConstants() const;

		Float3 getWeights(const Float4& constants, const Float2& point) const;
		Float3 getWeights(const Float3& position) const;

		bool isInTriangle(const Float3& weights) const;

		float interpolate(const Float3& weights, const Float3& values) const;
		Vertex interpolate(const Float3& weights) const;
	};

	// std::cout
	std::ostream& operator<<(std::ostream& os, const Triangle& obj);
}