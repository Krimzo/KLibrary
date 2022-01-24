#pragma once


namespace kl {
	struct renderer2D {
		/* Time */
		float deltaT = 0;
		float elapsedT = 0;

		/* View */
		kl::color background = kl::colors::gray;

		/* User functions */
		std::function<void()> setup = []() {};
		std::function<void(kl::keys*, kl::mouse*)> input = [](kl::keys* k, kl::mouse* m) {};
		std::function<void()> update = []() {};

		// Creates and runs a new engine
		void createNew(kl::ivec2 frameSize) {
			/* Engine timer */
			kl::timer timer;

			/* Default shaders */
			kl::shaders* default_sha = nullptr;
			kl::uniform w_uni;

			/* Window start definition */
			win.start = [&]() {
				/* Compiling default shaders */
				default_sha = new kl::shaders(
					kl::shaders::parse("res/shaders/renderer2D.sha", kl::shaders::Vertex),
					kl::shaders::parse("res/shaders/renderer2D.sha", kl::shaders::Fragment)
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
				elapsedT = timer.swElapsed();

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
		void stop() {
			win.stop();
		}

		// Sets the fullscreen mode
		void setFullscreen(bool enable) {
			win.setFullscreen(enable);
			win.resetViewport();
		}

		// Returns the frame size
		kl::ivec2 frameSize() {
			return win.getSize();
		}

		// Returns the frame center
		kl::ivec2 frameCenter() {
			return win.getCenter();
		}

		// Returns the aspect ratio
		float getAspect() {
			return win.getAspect();
		}

		// Creates a mesh
		kl::mesh* newMesh(std::string filePath, bool flipZ = true) {
			meshes.push_back(new kl::mesh(filePath, flipZ, true));
			return meshes.back();
		}
		kl::mesh* newMesh(std::vector<kl::vertex2D>& vertexData) {
			meshes.push_back(new kl::mesh(vertexData));
			return meshes.back();
		}

		// Deletes a mesh
		bool delMesh(kl::mesh* mesAddress) {
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
		kl::texture* newTexture(kl::image& image) {
			textures.push_back(new kl::texture(image));
			return textures.back();
		}
		kl::texture* newTexture(kl::image&& image) {
			textures.push_back(new kl::texture(image));
			return textures.back();
		}

		// Deletes a texture
		bool delTex(kl::texture* texAddress) {
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
		kl::object2D* newObject(kl::mesh* mes, kl::texture* tex) {
			objects.push_back(new kl::object2D(mes, tex));
			return objects.back();
		}

		// Deletes a game object
		bool delObject(kl::object2D* objectAddress) {
			for (int i = 0; i < objects.size(); i++) {
				if (objects[i] == objectAddress) {
					delete objects[i];
					objects.erase(objects.begin() + i);
					return true;
				}
			}
			return false;
		}

	private:
		// Window
		kl::window win;

		// Mesh buffer
		std::vector<kl::mesh*> meshes;

		// Texture buffer
		std::vector<kl::texture*> textures;

		// Object buffer
		std::vector<kl::object2D*> objects;
	};
}
