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
		// Shadow buffers
		//int depthFB = NULL;
		//int depthMap = NULL;

		// Light vp matrix
		kl::mat4 sunVP;

		// Shadow map size
		int mapSize = 0;

		// Shadow shaders
		//kl::glsl* depth_sha = nullptr;
		//kl::uniform sunVP_uni;
		//kl::uniform w_uni;

		// Shadow shader sources
		const static std::string vertSource;
		const static std::string fragSource;

	public:
		kl::vec3 color;
		kl::vec3 direction;
		float intensity = 0;

		// Constructor
		direct();
		
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

		void render(kl::window* win, const std::function<void()>& toRender) const;

		// Sets the object world transform matrix
		void setWMat(const kl::mat4& wMat) const;

		// Binds the shadow map
		void bindMap(int textureID) const;
	};
}
