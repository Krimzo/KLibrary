#pragma once

#include <iostream>
#include <functional>

#include "KrimzLib/dx/mesh.h"
#include "KrimzLib/dx/texture.h"
#include "KrimzLib/dx/shaders.h"
#include "KrimzLib/renderer/camera.h"
#include "KrimzLib/renderer/skybox.h"
#include "KrimzLib/renderer/entity.h"
#include "KrimzLib/renderer/light/ambient.h"
#include "KrimzLib/renderer/light/direct.h"
#include "KrimzLib/random.h"
#include "KrimzLib/time.h"


namespace kl {
	class renderer3D {
	private:
		// Window
		kl::window win;

		// Mesh buffer
		std::vector<kl::mesh*> meshes;

		// Texture buffer
		std::vector<kl::texture*> textures;

		// Object buffer
		std::vector<kl::entity*> objects;

		// Engine timer
		kl::timer timer;

		// Default shaders
		//kl::glsl* default_sha = nullptr;
		//kl::uniform w_uni;
		//kl::uniform vp_uni;
		//kl::uniform dark_uni;
		//kl::uniform sunL_uni;
		//kl::uniform sunD_uni;
		//kl::uniform sunVP_uni;

	public:
		// Time
		float deltaT = 0;
		float elapsedT = 0;

		// View
		kl::color background = kl::colors::gray;
		kl::camera camera;

		// Lighting
		kl::ambient ambient;
		kl::direct sun;

		// Engine skybox
		kl::skybox* skybox = nullptr;

		// User functions
		std::function<void()> setup;
		std::function<void(kl::keys*, kl::mouse*)> input;
		std::function<void()> update;

		// Constructor
		renderer3D();

		// Creates and runs a new engine
		void startNew(const kl::ivec2& frameSize);
		void stop() const;

		// Sets the fullscreen mode
		void setFullscreen(bool enable);

		// Returns the frame size
		kl::ivec2 frameSize() const;

		// Returns the frame center
		kl::ivec2 frameCenter() const;

		// Returns the aspect ratio
		float getAspect() const;

		// Mesh
		kl::mesh* newMesh(const std::string& filePath, bool flipZ = true);
		kl::mesh* newMesh(const std::vector<kl::vertex>& vertexData);
		void delMesh(kl::mesh* mesAddress);

		// Texture
		kl::texture* newTexture(const kl::image& image);
		void delTex(kl::texture* texAddress);

		// Entity
		kl::entity* newObject(kl::mesh* mes, kl::texture* tex);
		void delObject(kl::entity* objectAddress);
	};
}
