#pragma once 

#include "Math/Math.h"


namespace kl {
	struct Vertex {
		Float3 world;
		Float2 texture;
		Float3 normal;

		Vertex();
		Vertex(const Float3& world);
		Vertex(const Float3& world, const Float2& texture);
		Vertex(const Float3& world, const Float3& normal);
		Vertex(const Float3& world, const Float2& texture, const Float3& normal);
	};

	// std::cout
	std::ostream& operator<<(std::ostream& os, const Vertex& vertex);
}