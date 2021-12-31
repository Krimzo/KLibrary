#pragma once


namespace kl {
	namespace dx {
		struct vertCBuffStruc {
			kl::mat4 world;
			kl::mat4 viewProj;
		};

		struct renderer {
			/* Time */
			float deltaT = 0;
			float elapsedT = 0;

			/* View */
			kl::color background = kl::constant::colors::gray;
			kl::camera cam;

			/* Rendering */
			bool vSync = false;

			/* Lighting */


			/* User functions */
			std::function<void()> setup = []() {};
			std::function<void(kl::keys*, kl::mouse*)> input = [](kl::keys* k, kl::mouse* m) {};
			std::function<void()> update = []() {};

			// Creates and runs a new engine
			void createNew(kl::ivec2 frameSize) {
				/* Default shaders */
				kl::dx::shaders* defSha = nullptr;

				/* Shader constant buffers */
				kl::dx::cbuffer* vertCBuff = nullptr;

				/* Texture sampler */
				kl::dx::sampler* defSamp = nullptr;

				/* Engine timer */
				kl::time timer;
				
				/* Window start definition */
				win.start = [&]() {
					/* Gpu creation */
					GPU = new kl::dx::gpu(&win, 4);

					/* Creating default shaders */
					defSha = GPU->newShaders("res/shaders/dx/default.hlsl", "vShader", "pShader");
					GPU->bindShaders(defSha);

					/* Creating a vertex cbuffer */
					vertCBuff = GPU->newCBuffer(sizeof(vertCBuffStruc));
					GPU->bindCBuff(vertCBuff, 0);

					/* Creating default texture sampler */
					defSamp = GPU->newSampler();
					GPU->bindSampler(defSamp, 0);					

					/* Setting up the camera */
					cam.width = frameSize.x;
					cam.height = frameSize.y;
					cam.nearPlane = 0.01;
					cam.farPlane = 100;
					cam.sens = 0.025;
					cam.shadowD = 15;

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

					/* Temp cbufferstruct */
					vertCBuffStruc CBuffData = {};

					/* Setting the camera matrices */
					CBuffData.viewProj = cam.matrix();

					/* Clearing the default buffers */
					GPU->clear(background);

					/* Rendering objects */
					for (int i = 0; i < objects.size(); i++) {
						if (objects[i]->visible) {
							// Setting the world matrix
							CBuffData.world = objects[i]->geometry.matrix();

							// Setting the cbuffer data
							vertCBuff->setData(&CBuffData);

							// Rendering the object
							objects[i]->dx_render(GPU);
						}
					}

					/* Updating the fps display */
					win.setTitle(std::to_string(int(1 / deltaT)));

					/* Swapping the frame buffers */
					GPU->swap(vSync);
				};

				/* Window end definition */
				win.end = [&]() {
					// Deleting sampler
					delete defSamp;

					// Deleting shaders
					delete defSha;

					// Deleting cbuffers
					delete vertCBuff;

					// Deleting the gpu
					delete GPU;

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
				win.startNew(frameSize, kl::random::getString(6), false, true);
			}
			void stop() {
				win.stop();
			}

			// Sets the render mode
			void wireframe(bool enabled) {
				GPU->setRaster(enabled);
			}

			// Returns the frame center
			kl::ivec2 frameCenter() {
				return win.getCenter();
			}

			// Creates a new skybox
			void newSkybox(kl::image& front, kl::image& back, kl::image& left, kl::image& right, kl::image& top, kl::image& bottom) {
				delSkybox();
				sky = new kl::dx::skybox(front, back, left, right, top, bottom);
			}
			void newSkybox(kl::image&& front, kl::image&& back, kl::image&& left, kl::image&& right, kl::image&& top, kl::image&& bottom) {
				delSkybox();
				sky = new kl::dx::skybox(front, back, left, right, top, bottom);
			}

			// Deletes an existing skybox
			void delSkybox() {
				if (sky) {
					delete sky;
					sky = nullptr;
				}
			}

			// Creates a mesh
			kl::dx::mesh* newMesh(std::string filePath, bool flipZ = true) {
				meshes.push_back(GPU->newMesh(filePath, flipZ));
				return meshes.back();
			}
			kl::dx::mesh* newMesh(std::vector<kl::vertex>& vertexData) {
				meshes.push_back(GPU->newMesh(vertexData));
				return meshes.back();
			}

			// Deletes a mesh
			bool delMesh(kl::dx::mesh* mesAddress) {
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
			kl::dx::texture* newTexture(kl::image& image) {
				textures.push_back(GPU->newTexture(image));
				return textures.back();
			}
			kl::dx::texture* newTexture(kl::image&& image) {
				textures.push_back(GPU->newTexture(image));
				return textures.back();
			}

			// Deletes a texture
			bool delTex(kl::dx::texture* texAddress) {
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
			kl::renderable* newObject(kl::dx::mesh* mes, kl::dx::texture* tex) {
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

			// GPU
			kl::dx::gpu* GPU = nullptr;

			// Engine skybox
			kl::dx::skybox* sky = nullptr;

			// Mesh buffer
			std::vector<kl::dx::mesh*> meshes;

			// Texture buffer
			std::vector<kl::dx::texture*> textures;

			// Object buffer
			std::vector<kl::renderable*> objects;
		};
	}
}

