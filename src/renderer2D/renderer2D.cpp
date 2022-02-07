#include "KrimzLib/renderer2D/renderer2D.h"


// Constructor
kl::renderer2D::renderer2D() {
	setup = []() {};
	input = [](kl::keys* k, kl::mouse* m) {};
	update = []() {};
}

// Creates and runs a new engine
void kl::renderer2D::createNew(const kl::ivec2& frameSize) {
	/* Engine timer */
	kl::timer timer;

	/* Default shaders */
	kl::shaders* default_sha = nullptr;
	kl::uniform w_uni;

	/* Window start definition */
	win.start = [&]() {
		/* Compiling default shaders */
		default_sha = new kl::shaders(
			kl::shaders::parse("res/shaders/renderer2D.glsl", kl::shaders::Vertex),
			kl::shaders::parse("res/shaders/renderer2D.glsl", kl::shaders::Fragment)
		);

		/* Getting object shader uniforms */
		w_uni = default_sha->getUniform("w");

		/* Calling the user start */
		setup();
	};

	/* Window update definition */
	win.update = [&]() {
		/* Time calculations */
		deltaT = timer.interval();
		elapsedT = timer.elapsed();

		/* Clearing the default buffers */
		kl::gl::clearBuffers(background);

		/* Calling the user input */
		input(&win.keys, &win.mouse);

		/* Calling the physics update */
		for (int i = 0; i < objects.size(); i++) {
			objects[i]->upPhys(deltaT);
		}

		/* Calling the user update */
		update();

		/* Getting the window aspect */
		kl::mat3 arf = kl::mat3::scale(vec2(1 / win.getAspect(), 1));

		/* Rendering objects */
		for (int i = 0; i < objects.size(); i++) {
			if (objects[i]->visible) {
				// Setting the world matrix
				w_uni.setData(arf * objects[i]->matrix());

				// Rendering the object
				objects[i]->render();
			}
		}

		/* Updating the fps display */
		win.setTitle(std::to_string(int(1 / deltaT)));

		/* Swapping the frame buffers */
		win.swapBuffers();
	};

	/* Window end definition */
	win.end = [&]() {
		// Deleting shaders
		delete default_sha;

		// Deleting meshes
		for (int i = 0; i < meshes.size(); i++) {
			delete meshes[i];
		}
		meshes.clear();

		// Deleting textures
		for (int i = 0; i < textures.size(); i++) {
			delete textures[i];
		}
		textures.clear();
	};

	/* Window creation */
	timer.reset();
	win.startNew(frameSize, kl::random::STRING(6), false, true, true);
}
void kl::renderer2D::stop() {
	win.stop();
}

// Sets the fullscreen mode
void kl::renderer2D::setFullscreen(bool enable) {
	win.setFullscreen(enable);
	win.resetViewport();
}

// Returns the frame size
kl::ivec2 kl::renderer2D::frameSize() const {
	return win.getSize();
}

// Returns the frame center
kl::ivec2 kl::renderer2D::frameCenter() const {
	return win.getCenter();
}

// Returns the aspect ratio
float kl::renderer2D::getAspect() const {
	return win.getAspect();
}

// Creates a mesh
kl::mesh* kl::renderer2D::newMesh(const std::string& filePath, bool flipZ) {
	meshes.push_back(new kl::mesh(filePath, flipZ, true));
	return meshes.back();
}
kl::mesh* kl::renderer2D::newMesh(const std::vector<kl::vertex2D>& vertexData) {
	meshes.push_back(new kl::mesh(vertexData));
	return meshes.back();
}

// Deletes a mesh
bool kl::renderer2D::delMesh(kl::mesh* mesAddress) {
	for (int i = 0; i < meshes.size(); i++) {
		if (meshes[i] == mesAddress) {
			delete meshes[i];
			meshes.erase(meshes.begin() + i);
			return true;
		}
	}
	return false;
}

// Creates a texture
kl::texture* kl::renderer2D::newTexture(const kl::image& image) {
	textures.push_back(new kl::texture(image));
	return textures.back();
}

// Deletes a texture
bool kl::renderer2D::delTex(kl::texture* texAddress) {
	for (int i = 0; i < textures.size(); i++) {
		if (textures[i] == texAddress) {
			delete textures[i];
			textures.erase(textures.begin() + i);
			return true;
		}
	}
	return false;
}

// Creates a new game object
kl::object2D* kl::renderer2D::newObject(kl::mesh* mes, kl::texture* tex) {
	objects.push_back(new kl::object2D(mes, tex));
	return objects.back();
}

// Deletes a game object
bool kl::renderer2D::delObject(kl::object2D* objectAddress) {
	for (int i = 0; i < objects.size(); i++) {
		if (objects[i] == objectAddress) {
			delete objects[i];
			objects.erase(objects.begin() + i);
			return true;
		}
	}
	return false;
}
