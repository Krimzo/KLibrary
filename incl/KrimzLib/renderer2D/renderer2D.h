#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <functional>

#include "KrimzLib/renderer2D/object2D.h"
#include "KrimzLib/graphics/color.h"
#include "KrimzLib/window/window.h"
#include "KrimzLib/gl/shaders.h"
#include "KrimzLib/random.h"
#include "KrimzLib/time.h"


namespace kl {
	class renderer2D {
	private:
		// Window
		kl::window win;

		// Mesh buffer
		std::vector<kl::mesh*> meshes;

		// Texture buffer
		std::vector<kl::texture*> textures;

		// Object buffer
		std::vector<kl::object2D*> objects;

	public:
		/* Time */
		float deltaT = 0;
		float elapsedT = 0;

		/* View */
		kl::color background = kl::colors::gray;

		/* User functions */
		std::function<void()> setup;
		std::function<void(kl::keys*, kl::mouse*)> input;
		std::function<void()> update;

		// Constructor
		renderer2D();

		// Creates and runs a new engine
		void createNew(const kl::ivec2& frameSize);
		void stop();

		// Sets the fullscreen mode
		void setFullscreen(bool enable);

		// Returns the frame size
		kl::ivec2 frameSize() const;

		// Returns the frame center
		kl::ivec2 frameCenter() const;

		// Returns the aspect ratio
		float getAspect() const;

		// Creates a mesh
		kl::mesh* newMesh(const std::string& filePath, bool flipZ = true);
		kl::mesh* newMesh(const std::vector<kl::vertex2D>& vertexData);

		// Deletes a mesh
		bool delMesh(kl::mesh* mesAddress);

		// Creates a texture
		kl::texture* newTexture(const kl::image& image);

		// Deletes a texture
		bool delTex(kl::texture* texAddress);

		// Creates a new game object
		kl::object2D* newObject(kl::mesh* mes, kl::texture* tex);

		// Deletes a game object
		bool delObject(kl::object2D* objectAddress);
	};
}
