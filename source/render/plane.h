#pragma once

#include "math/math.h"


namespace kl
{
	struct plane
	{
		kl::float3 normal;
		kl::float3 point;

		plane();
		plane(const kl::float3& normal, const kl::float3& point);
	};

	// std::cout
	std::ostream& operator<<(std::ostream& os, const kl::plane& obj);
}
