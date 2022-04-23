#pragma once

#include <windows.h>
#include "math/float2.h"
#include "math/float3.h"
#include "math/float4.h"


namespace kl {
	namespace math {
		// PI
		inline const float pi = 3.14159265358979f;

		// Returns x from the given y of the line that goes through points a and b
		float lineX(const kl::float2& a, const kl::float2& b, float y);

		// Returns y from the given x of the line that goes through points a and b
		float lineY(const kl::float2& a, const kl::float2& b, float x);

		// Euluer/quat angles
		kl::float4 eulToQuat(const kl::float3& eul);
		kl::float3 quatToEul(const kl::float4& quat);
	};
}
