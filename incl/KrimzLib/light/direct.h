#pragma once

#include <functional>
#include <vector>

#include "KrimzLib/dx/shaders.h"
#include "KrimzLib/dx/sbuffer.h"
#include "KrimzLib/convert.h"
#include "KrimzLib/vecmath/vec4.h"
#include "KrimzLib/vecmath/mat4.h"
#include "KrimzLib/renderer/camera.h"
#include "KrimzLib/window/window.h"


namespace kl {
	class direct {
	public:
		kl::sbuffer* shadowMap = nullptr;
		kl::vec4 color = kl::colors::white;
		kl::vec3 direction = kl::vec3(0.0f, -1.0f, -2.0f);
		float intensity = 1.0f;
		
		// Returns the true light color
		kl::vec4 getCol() const;

		// Returns the direction of light
		kl::vec3 getDir() const;

		// Returns the light vp matrix
		kl::mat4 matrix(const kl::camera& cam) const;
	};
}
