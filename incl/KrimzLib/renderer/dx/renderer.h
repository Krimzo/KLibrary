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

				/* Engine timer */
				kl::time timer;
				
				/* Window start definition */
				win.start = [&]() {
					/* Gpu creation */
					dx_gpu = new kl::dx::gpu(&win, 4);

					/* Default shaders */
					defSha = dx_gpu->newShaders("res/shaders/dx/default.hlsl", "vShader", "pShader");
					dx_gpu->bindShaders(defSha);

					/* Creating a vertex cbuffer */
					vertCBuff = dx_gpu->newCBuffer(sizeof(vertCBuffStruc));
					dx_gpu->bindCBuff(vertCBuff);

					/* Setting up the depth testing */
					

					/* Setting up the camera */
					cam.width = frameSize.x;
					cam.height = frameSize.y;
					cam.nearPlane = 0.01;
					cam.farPlane = 100;
					cam.sens = 0.025;
					cam.shadowD = 15;

					/* Setting up the lights */


					/* Generating sun buffers */


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

					/* Calculating the light vp matrix */


					/* Setting the light uniforms */


					/* Rendering the shadows */


					/* Clearing the default buffers */
					dx_gpu->clear(background);

					/* Rendering objects */
					for (int i = 0; i < objects.size(); i++) {
						if (objects[i]->visible) {
							// Setting the world matrix
							CBuffData.world = objects[i]->geometry.matrix();

							// Setting the cbuffer data
							vertCBuff->setData(&CBuffData);

							// Rendering the object
							objects[i]->dx_render();
						}
					}

					/* Rendering skybox */


					/* Updating the fps display */
					win.setTitle(std::to_string(int(1 / deltaT)));

					/* Swapping the frame buffers */
					dx_gpu->swap();
				};

				/* Window end definition */
				win.end = [&]() {
					// Deleting shaders
					delete defSha;

					// Deleting cbuffers
					delete vertCBuff;

					// Deleting the gpu
					delete dx_gpu;

					// Deleting the sun shadow buffers


					// Deleting meshes
					for (int i = 0; i < meshes.size(); i++) {
						delete meshes[i];
					}
					meshes.clear();

					// Deleting textures
					//for (int i = 0; i < textures.size(); i++) {
					//	delete textures[i];
					//}
					//textures.clear();
				};

				/* Window creation */
				timer.getElapsed();
				timer.stopwatchReset();
				win.startNew(frameSize, kl::random::getString(6), false, true);
			}
			void stop() {
				win.stop();
			}

			// Returns the frame center
			kl::ivec2 frameCenter() {
				return win.getCenter();
			}

			// Creates a new skybox
			//void newSkybox(kl::image& front, kl::image& back, kl::image& left, kl::image& right, kl::image& top, kl::image& bottom) {
			//
			//}
			//void newSkybox(kl::image&& front, kl::image&& back, kl::image&& left, kl::image&& right, kl::image&& top, kl::image&& bottom) {
			//
			//}

			// Deletes an existing skybox
			//void delSkybox() {
			//
			//}

			// Creates a mesh
			kl::dx::mesh* newMesh(std::string filePath, bool flipZ = true) {
				meshes.push_back(dx_gpu->newMesh(filePath, flipZ));
				return meshes.back();
			}
			kl::dx::mesh* newMesh(std::vector<kl::vertex>& vertexData) {
				meshes.push_back(dx_gpu->newMesh(vertexData));
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
			//void newTexture(kl::image& image) {
			//
			//}
			//void newTexture(kl::image&& image) {
			//
			//}

			// Deletes a texture
			//bool delTex(kl::dx::texture* texAddress) {
			//	for (int i = 0; i < textures.size(); i++) {
			//		if (textures[i] == texAddress) {
			//			delete textures[i];
			//			textures.erase(textures.begin() + i);
			//			return true;
			//		}
			//	}
			//	return false;
			//}

			// Creates a new game object
			kl::renderable* newObject(kl::dx::mesh* mes) {
				objects.push_back(new kl::renderable(dx_gpu, mes));
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
			kl::dx::gpu* dx_gpu = nullptr;

			// Engine skybox


			// Mesh buffer
			std::vector<kl::dx::mesh*> meshes;

			// Texture buffer
			//std::vector<kl::dx::texture*> textures;

			// Object buffer
			std::vector<kl::renderable*> objects;
		};
	}
}

