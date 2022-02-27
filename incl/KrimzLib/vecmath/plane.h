#pragma once

#include "KrimzLib/vecmath/vec3.h"


namespace kl {
	class plane {
	public:
		kl::vec3 normal;
		kl::vec3 point;

		// Constructors
		plane();
		plane(const kl::vec3& normal, const kl::vec3& point);
	};
}
