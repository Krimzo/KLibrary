#pragma once

#include <iostream>
#include <functional>

#include "KrimzLib/dx/gpu.h"
#include "KrimzLib/dx/fbuffer.h"
#include "KrimzLib/dx/raster.h"
#include "KrimzLib/dx/shaders.h"
#include "KrimzLib/dx/cbuffer.h"
#include "KrimzLib/dx/mesh.h"
#include "KrimzLib/dx/texture.h"
#include "KrimzLib/dx/sampler.h"
#include "KrimzLib/renderer/camera.h"
#include "KrimzLib/renderer/skybox.h"
#include "KrimzLib/renderer/entity.h"
#include "KrimzLib/renderer/light/ambient.h"
#include "KrimzLib/renderer/light/direct.h"
#include "KrimzLib/random.h"
#include "KrimzLib/time.h"


namespace kl {
	struct VS_CB {
		kl::mat4 w;
		kl::mat4 vp;
	};

	class renderer {
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

		// DirectX
		kl::gpu* gpu = nullptr;
		kl::raster* solid_ra = nullptr;
		kl::raster* wire_ra = nullptr;
		kl::shaders* default_sh = nullptr;
		kl::cbuffer* default_cb = nullptr;
		kl::VS_CB default_cb_data;

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
		//kl::direct sun;

		// Engine skybox
		//kl::skybox* skybox = nullptr;

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

		// Sets the raster type
		void setWireframe(bool enabled);

		// Mesh
		kl::mesh* newMesh(const std::vector<kl::vertex>& vertexData);
		kl::mesh* newMesh(const std::string& filePath, bool flipZ = true);
		void delMesh(kl::mesh* mesAddress);

		// Texture
		kl::texture* newTexture(const kl::image& image);
		void delTexture(kl::texture* texAddress);

		// Entity
		kl::entity* newEntity(kl::mesh* mes, kl::texture* tex);
		void delEntity(kl::entity* objectAddress);
	};
}
