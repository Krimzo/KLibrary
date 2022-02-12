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
		// Light vp matrix
		kl::mat4 sunVP;

		// Shadow map size
		int mapSize = 0;

	public:
		kl::vec3 color;
		kl::vec3 direction;
		float intensity = 0;
		
		// Returns the true light color
		kl::vec3 getCol() const;

		// Returns the direction of light
		kl::vec3 getDir() const;

		// Generates the shadow buffers
		void genBuff(int mapSize);

		// Destroys the shadow buffers
		void delBuff();

		// Calculates the light vp matrix
		void calcMat(const kl::camera& cam);

		// Returns the light vp matrix
		kl::mat4 matrix() const;

		// Renders the shadows
		void render(kl::window* win, const std::function<void()>& toRender) const;
	};
}
