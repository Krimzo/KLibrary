#pragma once

#include <functional>
#include <vector>

#include "KrimzLib/dx/shaders.h"
#include "KrimzLib/convert.h"
#include "KrimzLib/vecmath/vec3.h"
#include "KrimzLib/vecmath/mat4.h"
#include "KrimzLib/renderer/camera.h"
#include "KrimzLib/window/window.h"


namespace kl {
	class direct {
	private:
		// Shadow map size
		int mapSize = 0;

	public:
		kl::vec3 color = kl::colors::white;
		kl::vec3 direction = kl::vec3(0.0f, -1.0f, -2.0f);
		float intensity = 1.0f;
		
		// Returns the true light color
		kl::vec3 getCol() const;

		// Returns the direction of light
		kl::vec3 getDir() const;

		// Generates the shadow buffers
		void genBuff(int mapSize);

		// Destroys the shadow buffers
		void delBuff();

		// Returns the light vp matrix
		kl::mat4 matrix(const kl::camera& cam) const;

		// Renders the shadows
		void renderShadows() const;
	};
}
