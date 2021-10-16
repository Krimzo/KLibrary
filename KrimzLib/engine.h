#pragma once


namespace kl
{
	class engine
	{
	public:
		// Engine properties
		double fpsLimit = 60;
		double deltaTime = 0;
		double gravity = 5;
		colorf background = {};
		camera engineCamera = {};

		// Outside functions that user defines
		std::function<void(void)> EngineStart = []() {};
		std::function<void(char)> EngineInput = [](char input) {};
		std::function<void(void)> EngineUpdate = []() {};

		// Creates the engine
		void Start(int width, int height, const wchar_t* name, double fov = 60)
		{
			engineWindow.WindowStart = [&]()
			{
				/* Enable 3D and depth buffer */
				opengl::Enabled3D(fov, engineWindow.GetWidth(), engineWindow.GetHeight());

				/* Enable textures */
				opengl::EnableTextures();

				/* Game start function call */
				EngineStart();

				/* First stopwatch reset */
				engineTime.StopwatchReset();
			};

			engineWindow.WindowUpdate = [&]()
			{
				/* Game input */
				EngineInput((char)engineWindow.KEY);

				/* Game logic */
				EngineUpdate();

				/* Apply physics */
				ObjectPhysics();

				/* Buffer clearing */
				opengl::ClearBuffers(background);

				/* Update camera rotation and position */
				opengl::UpdateCamera(engineCamera.position, engineCamera.rotation);

				/* Render all game triangles */
				static std::map<std::string, gameobject>::iterator itr;
				for (itr = engineObjects.begin(); itr != engineObjects.end(); itr++)
				{
					if (itr->second.physics)
					{
						opengl::RenderTriangles(itr->second.triangles, itr->second.position, itr->second.rotation, itr->second.size, itr->second.texture);

					}
				}

				/* Swap front and back frame buffers */
				opengl::FlipBuffers(engineWindow.GetHDC());

				/* Delta time calculation */
				double wantedFrameTime = 1 / fpsLimit;
				do {
					deltaTime = engineTime.StopwatchElapsed();
				} while (deltaTime < wantedFrameTime);
				engineTime.StopwatchReset();

				/* Display the FPS */
				engineWindow.SetTitle(int(1 / deltaTime));
			};

			engineWindow.WindowEnd = [&]()
			{

			};

			engineWindow.Start(width, height, name, false, true, true);
		}
		void Stop()
		{
			engineWindow.Stop();
		}
		~engine()
		{
			this->Stop();
		}

		// Adds a new game object if the name doesn't already exist
		gameobject* NewGameObject(std::string objectName, texture textureID = 0)
		{
			if (!engineObjects.count(objectName))
			{
				engineObjects.insert(std::pair<std::string, gameobject>(objectName, { true, textureID }));
				return &engineObjects.at(objectName);
			}
			return NULL;
		}
		gameobject* NewGameObject(std::string objectName, std::wstring filePath, texture textureID)
		{
			if (!engineObjects.count(objectName))
			{
				// Load file
				std::stringstream ss = std::stringstream(file::ReadText(filePath));

				// Parse object data
				std::string fileLine;
				std::vector<vec3> xyzCoords;
				std::vector<vec2> uvCoords;
				std::vector<std::vector<point>> fileTriangles;
				while (std::getline(ss, fileLine))
				{
					std::istringstream iss(fileLine);
					std::string linePart = "";
					iss >> linePart;
					if (linePart == "v")
					{
						vec3 tempVertex = {};
						int spaceCoordCounter = 0;
						while (iss) {
							iss >> linePart;
							if (spaceCoordCounter == 0)
							{
								tempVertex.x = stod(linePart);
							}
							else if (spaceCoordCounter == 1)
							{
								tempVertex.y = stod(linePart);
							}
							else if (spaceCoordCounter == 2)
							{
								tempVertex.z = stod(linePart);
							}
							spaceCoordCounter++;
						}
						xyzCoords.push_back(tempVertex);
					}
					else if (linePart == "vt")
					{
						vec2 tempVertex = {};
						int textureCoordCounter = 0;
						while (iss)
						{
							iss >> linePart;
							if (textureCoordCounter == 0)
							{
								tempVertex.x = stod(linePart);
							}
							else if (textureCoordCounter == 1)
							{
								tempVertex.y = stod(linePart);
							}
							textureCoordCounter++;
						}
						uvCoords.push_back(tempVertex);
					}
					else if (linePart == "f")
					{
						std::vector<point> tempTriangle(3);
						int vertexCounter = 0;
						while (iss && vertexCounter < 3)
						{
							iss >> linePart;
							for (int i = 0; i < 2; i++)
							{
								size_t slashPosition = linePart.find('/');
								std::string dataAsString = linePart.substr(0, slashPosition);
								if (i == 0)
								{
									tempTriangle[vertexCounter].x = stoi(dataAsString) - 1;
								}
								else if (i == 1)
								{
									tempTriangle[vertexCounter].y = stoi(dataAsString) - 1;
								}
								linePart = linePart.substr(slashPosition + 1);
							}
							vertexCounter++;
						}
						fileTriangles.push_back(tempTriangle);
					}
				}

				// Create the game object with data
				gameobject tempObject = { true, textureID };
				for (int i = 0; i < fileTriangles.size(); i++)
				{
					tempObject.triangles.push_back({ {
						{
						xyzCoords[fileTriangles[i][0].x].x,
						xyzCoords[fileTriangles[i][0].x].y,
						xyzCoords[fileTriangles[i][0].x].z,
						uvCoords[fileTriangles[i][0].y].x,
						uvCoords[fileTriangles[i][0].y].y
						},
						{
						xyzCoords[fileTriangles[i][1].x].x,
						xyzCoords[fileTriangles[i][1].x].y,
						xyzCoords[fileTriangles[i][1].x].z,
						uvCoords[fileTriangles[i][1].y].x,
						uvCoords[fileTriangles[i][1].y].y
						},
						{
						xyzCoords[fileTriangles[i][2].x].x,
						xyzCoords[fileTriangles[i][2].x].y,
						xyzCoords[fileTriangles[i][2].x].z,
						uvCoords[fileTriangles[i][2].y].x,
						uvCoords[fileTriangles[i][2].y].y
						}
					}, true });
				}
				engineObjects.insert(std::pair<std::string, gameobject>(objectName, tempObject));
				return &engineObjects.at(objectName);
			}
			return NULL;
		}

		// Removes a game object with the given name
		bool DeleteGameObject(std::string objectName)
		{
			if (engineObjects.count(objectName))
			{
				engineObjects.erase(objectName);
				return true;
			}
			return false;		
		}

		// Returns a reference to a wanted game object
		gameobject* GetGameObject(std::string objectName)
		{
			if (engineObjects.count(objectName))
				return &engineObjects.at(objectName);
			return NULL;
		}

		// Adds a new texture to the engine memory
		texture NewTexture(bitmap& textureData)
		{
			texture createdID = 0;
			glGenTextures(1, &createdID);
			glBindTexture(GL_TEXTURE_2D, createdID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureData.GetWidth(), textureData.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData.GetPixelData());
			glGenerateMipmap(GL_TEXTURE_2D);
			return createdID;
		}
		texture NewTexture(bitmap&& textureData)
		{
			texture createdID = 0;
			glGenTextures(1, &createdID);
			glBindTexture(GL_TEXTURE_2D, createdID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureData.GetWidth(), textureData.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData.GetPixelData());
			glGenerateMipmap(GL_TEXTURE_2D);
			return createdID;
		}

	private:
		// Misc
		window engineWindow = window();

		// Time
		time engineTime = time();

		// Objects
		std::map<std::string, gameobject> engineObjects;

		// Computing object physics 
		void ObjectPhysics()
		{
			static std::map<std::string, gameobject>::iterator itr;
			for (itr = engineObjects.begin(); itr != engineObjects.end(); itr++)
			{
				if (itr->second.physics)
				{
					// Applying gravity
					itr->second.velocity.y -= gravity * itr->second.gravityMulti * deltaTime;

					// Applying velocity
					itr->second.position.x += itr->second.velocity.x * deltaTime;
					itr->second.position.y += itr->second.velocity.y * deltaTime;
					itr->second.position.z += itr->second.velocity.z * deltaTime;

					// Applying angular momentum
					itr->second.rotation.x += itr->second.angularMo.x * deltaTime;
					itr->second.rotation.y += itr->second.angularMo.y * deltaTime;
					itr->second.rotation.z += itr->second.angularMo.z * deltaTime;
				}
			}
		}
	};
}
