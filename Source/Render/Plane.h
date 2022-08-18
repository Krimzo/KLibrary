#pragma once

#include "Math/Math.h"


namespace kl {
	struct Plane {
		Float3 normal;
		Float3 point;

		Plane();
		Plane(const Float3& normal, const Float3& point);
	};

	// std::cout
	std::ostream& operator<<(std::ostream& os, const Plane& obj);
}
