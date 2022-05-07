#pragma once

#include <string>

#include "libspec/using.h"
#include "math/float2.h"
#include "math/float3.h"
#include "math/float4.h"
#include "graphics/color.h"


namespace kl {
	namespace convert {
		// Converts degrees to radians
		float toRadians(float degrees);
		kl::float2 toRadians(const kl::float2& degrees);
		kl::float3 toRadians(const kl::float3& degrees);

		// Converts radians to degrees
		float toDegrees(float radians);
		kl::float2 toDegrees(const kl::float2& radians);
		kl::float3 toDegrees(const kl::float3& radians);

		// Byte to float color
		float toFloCol(byte val);

		// Converts a float color to color
		kl::color toColor(const kl::float4& fCol);

		// Converts a string to a wstring
		std::wstring toWString(const String& data);
	};
}
