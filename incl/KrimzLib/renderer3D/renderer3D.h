#pragma once


namespace kl {
	struct renderer3D {
		/* Time */
		float deltaT = 0;
		float elapsedT = 0;

		/* View */
		kl::color background = kl::colors::gray;
		kl::camera cam;

		/* Lighting */
		kl::ambient dark;
		kl::direct sun;

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
			kl::uniform vp_uni;
			kl::uniform dark_uni;
			kl::uniform sunL_uni;
			kl::uniform sunD_uni;
			kl::uniform sunVP_uni;

			/* Window start definition */
			win.start = [&]() {
				/* Setting up the depth testing */
				kl::gl::setDepthTest(true);

				/* Setting up the camera */
				cam.nearPlane = 0.01f;
				cam.farPlane = 100;
				cam.sens = 0.025f;

				/* Setting up the lights */
				dark.color = kl::colors::white;
				dark.intensity = 0.1f;
				sun.color = kl::colors::white;
				sun.intensity = 1;
				sun.direction = kl::vec3(0, -1, -2);

				/* Compiling default shaders */
				default_sha = new kl::shaders(
					kl::shaders::parse("res/shaders/renderer3D.sha", kl::shaders::Vertex),
					kl::shaders::parse("res/shaders/renderer3D.sha", kl::shaders::Fragment)
				);

				/* Getting object shader uniforms */
				w_uni = default_sha->getUniform("w");
				vp_uni = default_sha->getUniform("vp");
				dark_uni = default_sha->getUniform("dark");
				sunL_uni = default_sha->getUniform("sunL");
				sunD_uni = default_sha->getUniform("sunD");
				sunVP_uni = default_sha->getUniform("sunVP");
				default_sha->getUniform("shadowMap").setData(1);

				/* Generating sun buffers */
				sun.genBuff(4096);

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

				/* Setting the camera uniforms */
				vp_uni.setData(cam.matrix());

				/* Calculating the light vp matrix */
				sun.calcMat(cam);

				/* Setting the light uniforms */
				dark_uni.setData(dark.getCol());
				sunL_uni.setData(sun.getCol());
				sunD_uni.setData(sun.getDir());
				sunVP_uni.setData(sun.matrix());

				/* Rendering the shadows */
				sun.render(&win, [&]() {
					for (int i = 0; i < objects.size(); i++) {
						if (objects[i]->shadows) {
							// Setting the world matrix
							sun.setWMat(objects[i]->matrix());

							// Rendering the object
							objects[i]->render();
						}
					}
				});

				/* Rendering objects */
				for (int i = 0; i < objects.size(); i++) {
					if (objects[i]->visible) {
						// Setting the world matrix
						w_uni.setData(objects[i]->matrix());

						// Rendering the object
						objects[i]->render();
					}
				}

				/* Rendering skybox */
				if (sky) sky->render(cam.matrix());

				/* Updating the fps display */
				win.setTitle(std::to_string(int(1 / deltaT)));

				/* Swapping the frame buffers */
				win.swapBuffers();
			};

			/* Window end definition */
			win.end = [&]() {
				// Deleting shaders
				delete default_sha;

				// Deleting the sun shadow buffers
				sun.delBuff();

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

		// Creates a new skybox
		void newSkybox(kl::image& front, kl::image& back, kl::image& left, kl::image& right, kl::image& top, kl::image& bottom) {
			delSkybox();
			sky = new kl::skybox(front, back, left, right, top, bottom);
		}
		void newSkybox(kl::image&& front, kl::image&& back, kl::image&& left, kl::image&& right, kl::image&& top, kl::image&& bottom) {
			delSkybox();
			sky = new kl::skybox(front, back, left, right, top, bottom);
		}

		// Deletes an existing skybox
		void delSkybox() {
			if (sky) {
				delete sky;
				sky = nullptr;
			}
		}

		// Creates a mesh
		kl::mesh* newMesh(std::string filePath, bool flipZ = true) {
			meshes.push_back(new kl::mesh(filePath, flipZ, false));
			return meshes.back();
		}
		kl::mesh* newMesh(std::vector<kl::vertex3D>& vertexData) {
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
		kl::object3D* newObject(kl::mesh* mes, kl::texture* tex) {
			objects.push_back(new kl::object3D(mes, tex));
			return objects.back();
		}

		// Deletes a game object
		bool delObject(kl::object3D* objectAddress) {
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

		// Engine skybox
		kl::skybox* sky = nullptr;

		// Mesh buffer
		std::vector<kl::mesh*> meshes;

		// Texture buffer
		std::vector<kl::texture*> textures;

		// Object buffer
		std::vector<kl::object3D*> objects;
	};
}
