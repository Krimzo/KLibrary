#pragma once

#include "KrimzLib/math/float4.h"


namespace kl {
	class ambient {
	public:
		kl::float4 color = kl::colors::white;
		float intensity = 0.1f;

		// Returns the true light color
		kl::float4 getCol() const;
	};
}
