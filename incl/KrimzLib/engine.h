#pragma once


namespace kl {
	class engine {
	public:
		// Engine properties
		double fpsLimit = -1;
		double deltaTime = 0;
		double gravity = 9.81;
		kl::colord background = {};
		kl::camera engineCamera = {};

		// Outside functions that user defines
		std::function<void(void)> EngineStart = []() {};
		std::function<void(kl::key key, kl::mouse mouse)> EngineInput = [](kl::key key, kl::mouse mouse) {};
		std::function<void(void)> EngineUpdate = []() {};

		// Constructor
		engine(kl::colord background = {}) {
			this->background = background;
		}

		// Creates the engine
		void StartNew(kl::size size, std::string name, double fov = 60) {
			engineWindow.WindowStart = [&]() {
				/* Enable 3D and depth buffer */
				kl::opengl::Enable3D(fov, engineWindow.GetSize());

				/* Enable textures */
				kl::opengl::EnableTextures();

				/* Game start function call */
				EngineStart();

				/* First stopwatch reset */
				engineTime.StopwatchReset();
			};

			engineWindow.WindowUpdate = [&]() {
				/* Game input */
				EngineInput(engineWindow.KEY, engineWindow.MOUSE);

				/* Game logic */
				EngineUpdate();

				/* Apply physics */
				PhysicsUpdate();

				/* Buffer clearing */
				kl::opengl::ClearBuffers(background);

				/* Update camera rotation and position */
				kl::opengl::UpdateCamera(engineCamera);

				/* Render all game triangles */
				for (objItr = engineObjects.begin(); objItr != engineObjects.end(); objItr++) {
					if (objItr->second.visible) {
						kl::opengl::RenderTriangles(objItr->second.triangles, objItr->second.position, objItr->second.rotation, objItr->second.size, objItr->second.texture);
					}
				}

				/* Swap front and back frame buffers */
				kl::opengl::SwapFrameBuffers(engineWindow.GetHDC());

				/* Delta time calculation */
				double wantedFrameTime = 1 / fpsLimit;
				do {
					deltaTime = engineTime.StopwatchElapsed();
				} while (deltaTime < wantedFrameTime);
				engineTime.StopwatchReset();

				/* Display the FPS */
				engineWindow.SetTitle(int(1 / deltaTime));
			};

			engineWindow.WindowEnd = [&]() {
				for (int i = 0; i < engineTextures.size(); i++) {
					kl::opengl::DeleteTexture(engineTextures[i]);
				}
			};

			engineWindow.StartNew(size, name, false, true, true);
		}
		void Stop() {
			engineWindow.Stop();
		}
		~engine() {
			this->Stop();
		}

		// Adds a new game object if the name doesn't already exist
		kl::gameobject* NewObject(std::string objectName, kl::texture textureID = 0) {
			if (!engineObjects.count(objectName)) {
				engineObjects.insert(std::pair<std::string, kl::gameobject>(objectName, kl::gameobject(textureID)));
				return &engineObjects.at(objectName);
			}
			return nullptr;
		}
		kl::gameobject* NewObject(std::string objectName, std::string filePath, kl::texture textureID) {
			if (!engineObjects.count(objectName)) {
				// Load file
				FILE* fileStream = fopen(filePath.c_str(), "r");
				if (!fileStream) {
					return nullptr;
				}

				// Data buffers
				kl::gameobject tempObject(textureID);
				std::vector<kl::vec3> xyzCoords = {};
				std::vector<kl::vec2> uvCoords = {};
				std::string tempBuffer; tempBuffer.resize(100);
				int scanStatus = 0;
				kl::vec3 tempXYZ = {};
				kl::vec2 tempUV = {};
				int coordIndex0 = 0;
				int textureIndex0 = 0;
				int normalIndx0 = 0;
				int coordIndex1 = 0;
				int textureIndex1 = 0;
				int normalIndx1 = 0;
				int coordIndex2 = 0;
				int textureIndex2 = 0;
				int normalIndx2 = 0;

				// Parse .obj data
				while (scanStatus != -1) {
					if ((scanStatus = fscanf(fileStream, "v %lf %lf %lf", &tempXYZ.x, &tempXYZ.y, &tempXYZ.z)) == 3) {
						xyzCoords.push_back(tempXYZ);
					}
					else if ((scanStatus = fscanf(fileStream, "t %lf %lf", &tempUV.x, &tempUV.y)) == 2) {
						uvCoords.push_back(tempUV);
					}
					else if ((scanStatus = fscanf(fileStream, "f %d/%d/%d %d/%d/%d %d/%d/%d", &coordIndex0, &textureIndex0, &normalIndx0, &coordIndex1, &textureIndex1, &normalIndx1, &coordIndex2, &textureIndex2, &normalIndx2)) == 9) {
						tempObject.triangles.push_back(
							kl::triangle(
								kl::vertex(xyzCoords[--coordIndex0].x, xyzCoords[coordIndex0].y, xyzCoords[coordIndex0].z, uvCoords[--textureIndex0].x, uvCoords[textureIndex0].y),
								kl::vertex(xyzCoords[--coordIndex1].x, xyzCoords[coordIndex1].y, xyzCoords[coordIndex1].z, uvCoords[--textureIndex1].x, uvCoords[textureIndex1].y),
								kl::vertex(xyzCoords[--coordIndex2].x, xyzCoords[coordIndex2].y, xyzCoords[coordIndex2].z, uvCoords[--textureIndex2].x, uvCoords[textureIndex2].y)
							)
						);
					}
					else {
						fgets(&tempBuffer[0], 100, fileStream);
					}
				}
				fclose(fileStream);

				// Save object in memory
				engineObjects.insert(std::pair<std::string, kl::gameobject>(objectName, tempObject));
				return &engineObjects.at(objectName);
			}
			return nullptr;
		}

		// Returns a reference to the wanted game object
		kl::gameobject* GetEngineObject(std::string objectName) {
			if (engineObjects.count(objectName)) {
				return &engineObjects.at(objectName);
			}
			return nullptr;
		}

		// Removes a game object with the given name
		bool DeleteObject(std::string objectName) {
			if (engineObjects.count(objectName)) {
				engineObjects.erase(objectName);
				return true;
			}
			return false;
		}

		// Adds a new texture to the engine
		kl::texture NewTexture(kl::image& textureImage) {
			kl::texture tempTex = kl::opengl::NewTexture(textureImage);
			engineTextures.push_back(tempTex);
			return tempTex;
		}
		kl::texture NewTexture(kl::image&& textureImage) {
			return NewTexture(textureImage);
		}

	private:
		kl::window engineWindow = {};
		kl::time engineTime = {};
		std::map<std::string, kl::gameobject> engineObjects = {};
		std::map<std::string, kl::gameobject>::iterator objItr = {};
		std::vector<kl::texture> engineTextures = {};

		// Computing object physics 
		void PhysicsUpdate() {
			for (objItr = engineObjects.begin(); objItr != engineObjects.end(); objItr++) {
				if (objItr->second.physics) {
					// Applying gravity
					objItr->second.velocity.y -= gravity * objItr->second.gravity * deltaTime;

					// Applying velocity
					objItr->second.position.x += objItr->second.velocity.x * deltaTime;
					objItr->second.position.y += objItr->second.velocity.y * deltaTime;
					objItr->second.position.z += objItr->second.velocity.z * deltaTime;

					// Applying angular momentum
					objItr->second.rotation.x += objItr->second.angular.x * deltaTime;
					objItr->second.rotation.y += objItr->second.angular.y * deltaTime;
					objItr->second.rotation.z += objItr->second.angular.z * deltaTime;
				}
			}
		}
	};
}
