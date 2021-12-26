#pragma once


namespace kl {
	struct renderer {
		/* Time */
		float deltaT = 0;
		float elapsedT = 0;

		/* View */
		kl::color background = kl::constant::colors::gray;
		kl::camera cam;

		/* Lighting */
		kl::ambient dark;
		kl::direct sun;

		/* User functions */
		std::function<void()> setup = []() {};
		std::function<void(kl::keys*, kl::mouse*)> input = [](kl::keys* k, kl::mouse* m) {};
		std::function<void()> update = []() {};

		// Creates and runs a new engine
		void createNew(kl::size frameSize) {
			/* Engine timer */
			kl::time timer;

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
				kl::opengl::setDepthTest(true);

				/* Setting up the camera */
				cam.aspect = (float)frameSize.width / frameSize.height;
				cam.nearPlane = 0.01;
				cam.farPlane = 100;
				cam.sens = 0.025;
				cam.shadowD = 15;

				/* Setting up the lights */
				dark.color = kl::constant::colors::white;
				dark.intensity = 0.1;
				sun.color = kl::constant::colors::white;
				sun.intensity = 1;
				sun.direction = kl::vec3(0, -1, -2);

				/* Compiling object shaders */
				default_sha = new kl::shaders(
					kl::file::readText("res/shaders/renderer.vert"),
					kl::file::readText("res/shaders/renderer.frag")
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
				deltaT = timer.getElapsed();
				elapsedT = timer.stopwatchElapsed();

				/* Calling the user input */
				input(&win.keys, &win.mouse);

				/* Calling the user update */
				update();

				/* Calling the physics update */
				for (int i = 0; i < objects.size(); i++) {
					objects[i]->upPhys(deltaT);
				}

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
				sun.render(frameSize, [&]() {
					for (int i = 0; i < objects.size(); i++) {
						if (objects[i]->visible) {
							// Setting the world matrix
							sun.setWMat(objects[i]->geometry.matrix());

							// Rendering the object
							objects[i]->render();
						}
					}
				});

				/* Clearing the default buffers */
				kl::opengl::clearBuffers(background);

				/* Rendering objects */
				for (int i = 0; i < objects.size(); i++) {
					if (objects[i]->visible) {
						// Setting the world matrix
						w_uni.setData(objects[i]->geometry.matrix());
				
						// Rendering the object
						objects[i]->render();
					}
				}

				/* Rendering skybox */
				if (sky) sky->render(cam.matrix());

				/* Updating the fps display */
				win.setTitle(std::to_string(int(1 / deltaT)));

				/* Swapping the frame buffers */
				win.swapFrames();
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
			timer.getElapsed();
			timer.stopwatchReset();
			win.startNew(frameSize, kl::random::getString(6), false, true, true);
		}
		void stop() {
			win.stop();
		}

		// Returns the frame center
		kl::point frameCenter() {
			return win.getCenter();
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
			meshes.push_back(new kl::mesh(filePath, flipZ));
			return meshes.back();
		}
		kl::mesh* newMesh(std::vector<kl::vertex>& vertexData) {
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
		kl::renderable* newObject(kl::mesh* mes, kl::texture* tex) {
			objects.push_back(new kl::renderable(mes, tex));
			return objects.back();
		}

		// Deletes a game object
		bool delObject(kl::renderable* objectAddress) {
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
		std::vector<kl::renderable*> objects;
	};
}
