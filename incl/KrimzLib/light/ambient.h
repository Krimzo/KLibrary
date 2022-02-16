#pragma once

#include "KrimzLib/vecmath/vec3.h"


namespace kl {
	class ambient {
	public:
		kl::vec3 color = kl::colors::white;
		float intensity = 0.1f;

		// Returns the true light color
		kl::vec3 getCol() const;
	};
}
