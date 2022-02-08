#pragma once

#include <iostream>
#include <functional>

#include "KrimzLib/gl/gl.h"
#include "KrimzLib/gl/mesh.h"
#include "KrimzLib/gl/texture.h"
#include "KrimzLib/gl/shaders.h"
#include "KrimzLib/renderer3D/camera.h"
#include "KrimzLib/renderer3D/skybox.h"
#include "KrimzLib/renderer3D/object3D.h"
#include "KrimzLib/renderer3D/light/ambient.h"
#include "KrimzLib/renderer3D/light/direct.h"
#include "KrimzLib/random.h"
#include "KrimzLib/time.h"


namespace kl {
	class renderer3D {
	private:
		// Window
		kl::window win;

		// Engine skybox
		kl::skybox* sky = nullptr;

		// Mesh buffer
		std::vector<kl::mesh*> meshes;

		// Texture buffer
		std::vector<kl::texture*> textures;

		// Object buffer
		std::vector<kl::object3D*> objects;

	public:
		// Time
		float deltaT = 0;
		float elapsedT = 0;

		// View
		kl::color background = kl::colors::gray;
		kl::camera cam;

		// Lighting
		kl::ambient dark;
		kl::direct sun;

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

		// Creates a new skybox
		void newSkybox(const kl::image& front, const kl::image& back, const kl::image& left, const kl::image& right, const kl::image& top, const kl::image& bottom);

		// Deletes an existing skybox
		void delSkybox();

		// Creates a mesh
		kl::mesh* newMesh(const std::string& filePath, bool flipZ = true);
		kl::mesh* newMesh(const std::vector<kl::vertex3D>& vertexData);

		// Deletes a mesh
		bool delMesh(kl::mesh* mesAddress);

		// Creates a texture
		kl::texture* newTexture(const kl::image& image);

		// Deletes a texture
		bool delTex(kl::texture* texAddress);

		// Creates a new game object
		kl::object3D* newObject(kl::mesh* mes, kl::texture* tex);

		// Deletes a game object
		bool delObject(kl::object3D* objectAddress);
	};
}
