#pragma once

#include "Math/Math.h"


namespace kl {
	struct Sphere {
		Float3 center;
		float radius = 0;

		Sphere();
		Sphere(const Float3& center, float radius);
	};

	// std::cout
	std::ostream& operator<<(std::ostream& os, const Sphere& obj);
}
