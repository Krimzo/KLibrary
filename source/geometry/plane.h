#pragma once

#include "math/float3.h"


namespace kl {
	class plane {
	public:
		kl::float3 normal;
		kl::float3 point;

		// Constructors
		plane();
		plane(const kl::float3& normal, const kl::float3& point);
	};
}
