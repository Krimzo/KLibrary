#pragma once

#include "KrimzLib/vecmath/vec3.h"


namespace kl {
	class ambient {
	public:
		kl::vec3 color;
		float intensity = 0;

		// Returns the true light color
		kl::vec3 getCol() const;
	};
}
