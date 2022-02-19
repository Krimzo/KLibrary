#pragma once

#include "KrimzLib/vecmath/vec4.h"


namespace kl {
	class ambient {
	public:
		kl::vec4 color = kl::colors::white;
		float intensity = 0.1f;

		// Returns the true light color
		kl::vec4 getCol() const;
	};
}
