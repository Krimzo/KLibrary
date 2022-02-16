#pragma once

#include <iostream>
#include <functional>

#include "KrimzLib/memory/pbuffer.h"
#include "KrimzLib/dx/gpu.h"
#include "KrimzLib/dx/fbuffer.h"
#include "KrimzLib/dx/raster.h"
#include "KrimzLib/dx/shaders.h"
#include "KrimzLib/dx/cbuffer.h"
#include "KrimzLib/dx/mesh.h"
#include "KrimzLib/dx/texture.h"
#include "KrimzLib/dx/sampler.h"
#include "KrimzLib/entity/entity.h"
#include "KrimzLib/light/ambient.h"
#include "KrimzLib/light/direct.h"
#include "KrimzLib/renderer/camera.h"
#include "KrimzLib/renderer/skybox.h"
#include "KrimzLib/random.h"
#include "KrimzLib/time.h"


namespace kl {
	class renderer {
	private:
		// Window
		kl::window win;

		// Entity buffer
		kl::pbuffer<kl::entity> entities;

		// Skybox buffer
		kl::pbuffer<kl::skybox> skyboxes;

		// Engine timer
		kl::timer timer;

		// DirectX
		kl::gpu* gpu = nullptr;
		kl::shaders* default_sh = nullptr;

	public:
		// Time
		float deltaT = 0;
		float elapsedT = 0;
		bool vSync = false;

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
		renderer();

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
		kl::mesh* newMesh(const std::vector<kl::vertex>& vertices);
		kl::mesh* newMesh(const std::string& filePath, bool flipZ = true);
		bool delMesh(kl::mesh* mes);

		// Texture
		kl::texture* newTexture(const kl::image& img);
		bool delTexture(kl::texture* tex);

		// Entity
		kl::entity* newEntity(kl::mesh* mes, kl::texture* tex);
		bool delEntity(kl::entity* ent);

		// Skybox
		kl::skybox* newSkybox(const kl::image& fullBox);
		kl::skybox* newSkybox(const kl::image& front, const kl::image& back, const kl::image& left, const kl::image& right, const kl::image& top, const kl::image& bottom);
		bool delSkybox(kl::skybox* sky);
	};
}
