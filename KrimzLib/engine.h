#pragma once
#include <functional>
#include <string>
#include <sstream>
#include <windows.h>
#include "KrimzLib/window.h"
#include "KrimzLib/time.h"
#include "KrimzLib/color.h"
#include "KrimzLib/constant.h"
#include "KrimzLib/opengl.h"


/* --- TYPES --- */
namespace kl
{
	struct gameobject
	{
		std::string name = "";
		std::vector<triangle> triangles = {};
		bool visible = true;
		vec3 position = {};
		vec3 size = { 1, 1, 1 };
		vec3 rotation = {};
		bool physics = false;
		double gravityMulti = 1;
		vec3 velocity = {};
		vec3 angularMo = {};
	};

	struct camera
	{
		vec3 position = {};
		vec3 rotation = {};
	};

	struct spotlight {
		color color = {};
		vec3 position = {};
		vec3 rotation = {};
	};
}


/* --- CLASS --- */
namespace kl
{
	class engine
	{
	public:
		// Engine properties
		double deltaTime = 0;
		double gravity = 10;
		colord background = {};
		camera engineCamera = {};

		// Outside functions that user defines
		std::function<void(void)> EngineStart = []() {};
		std::function<void(char)> EngineInput = [](char input) {};
		std::function<void(void)> EngineUpdate = []() {};

		// Creates the engine
		void Start(int width, int height, std::wstring name, double fov = 60)
		{
			if (!engineRunning)
			{
				// Misc
				engineRunning = true;
				engineWindow = new window(width, height, name.c_str(), false, true, true);
				engineWidth = engineWindow->GetWidth();
				engineHeight = engineWindow->GetHeight();

				engineWindow->OpenGLStart = [&]()
				{
					// Camera setup
					glMatrixMode(GL_PROJECTION);
					glLoadIdentity();
					gluPerspective(fov, (double)engineWidth / engineHeight, 0.01, 100.0);

					// Enable z buffer
					glEnable(GL_DEPTH_TEST);
					glDepthFunc(GL_LESS);
				};

				engineWindow->OpenGLUpdate = [&]()
				{
					// Handle texture creation/destruction
					if (createTexture)
					{
						id createdID;
						glGenTextures(1, &createdID);
						glBindTexture(GL_TEXTURE_2D, createdID);
						glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tempTextureWidth, tempTextureHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, tempTextureData);
						glGenerateMipmap(GL_TEXTURE_2D);
						tempTextureID = createdID;
						createTexture = false;
					}

					// Buffer clearing
					opengl::ClearBuffers(background);

					// Update camera rotation and position
					glMatrixMode(GL_MODELVIEW);
					glLoadIdentity();
					glRotated(engineCamera.rotation.x, 1, 0, 0);
					glRotated(engineCamera.rotation.y, 0, 1, 0);
					glRotated(engineCamera.rotation.z, 0, 0, 1);
					glTranslated(engineCamera.position.x, engineCamera.position.y, engineCamera.position.z);

					// Rendering all game triangles
					for (int i = 0; i < engineObjects.size(); i++)
					{
						if (engineObjects[i].visible)
						{
							for (int j = 0; j < engineObjects[i].triangles.size(); j++)
							{
								opengl::old::DrawTriangle(engineObjects[i].triangles[j], engineObjects[i].size, engineObjects[i].rotation, engineObjects[i].position);
							}
						}
					}

					// Flipping front and back frame buffers
					opengl::FlipBuffers(engineWindow->GetHDC());
				};
				
				// Start
				EngineLoop();
			}
		}

		// Destroyes the engine
		void Stop()
		{
			if (engineRunning)
			{
				engineRunning = false;
				delete engineWindow;
			}
		}
		~engine()
		{
			this->Stop();
		}

		// Adds a new game object if the name doesn't already exist
		gameobject* NewGameObject(std::string objectName)
		{
			for (int i = 0; i < engineObjects.size(); i++)
			{
				if (engineObjects[i].name == objectName)
				{
					printf("Game object \"%s\" already exists!\n", objectName.c_str());
					console::WaitFor(' ', true);
					exit(69);
				}
			}
			engineObjects.push_back({ objectName });
			return &engineObjects.back();
		}
		gameobject* NewGameObject(std::string objectName, std::wstring filePath, id textureID)
		{
			for (int i = 0; i < engineObjects.size(); i++)
			{
				if (engineObjects[i].name == objectName)
				{
					printf("Game object \"%s\" already exists!\n", objectName.c_str());
					console::WaitFor(' ', true);
					exit(69);
				}
			}

			// Load file
			std::stringstream ss = std::stringstream(file::ReadText(filePath));

			// Parse object data
			std::string fileLine;
			std::vector<vec3> xyzCoords;
			std::vector<vec2> uvCoords;
			std::vector<std::vector<point>> fileTriangles;
			while (std::getline(ss, fileLine)) {
				std::istringstream iss(fileLine);
				std::string linePart;
				iss >> linePart;
				if (linePart == "v") {
					vec3 tempVertex;
					int counter = 0;
					while (iss) {
						iss >> linePart;
						if (counter == 0) {
							tempVertex.x = stod(linePart);
						}
						else if (counter == 1) {
							tempVertex.y = stod(linePart);
						}
						else if (counter == 2) {
							tempVertex.z = stod(linePart);
						}
						counter++;
					}
					xyzCoords.push_back(tempVertex);
				}
				else if (linePart == "vt") {
					vec2 tempVertex;
					int counter = 0;
					while (iss) {
						iss >> linePart;
						if (counter == 0) {
							tempVertex.x = stod(linePart);
						}
						else if (counter == 1) {
							tempVertex.y = stod(linePart);
						}
						counter++;
					}
					uvCoords.push_back(tempVertex);
				}
				else if (linePart == "f") {
					std::vector<point> tempTriangle(3);
					int vertexCounter = 0;
					while (iss && vertexCounter < 3) {
						iss >> linePart;
						for (int i = 0; i < 2; i++) {
							size_t slashPosition = linePart.find('/');
							std::string dataAsString = linePart.substr(0, slashPosition);
							if (i == 0) {
								tempTriangle[vertexCounter].x = stoi(dataAsString) - 1;
							}
							else if (i == 1) {
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
			gameobject tempObject = { objectName };
			for (int i = 0; i < fileTriangles.size(); i++)
			{
				tempObject.triangles.push_back({{
					{
					xyzCoords[fileTriangles[i][0].x].x,
					xyzCoords[fileTriangles[i][0].x].y,
					xyzCoords[fileTriangles[i][0].x].z,
					uvCoords[fileTriangles[i][0].y].x,
					uvCoords[fileTriangles[i][0].y].y
					},
					{ xyzCoords[fileTriangles[i][1].x].x,
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
				}, true, textureID });
			}
			
			// Load the game object to the engine
			engineObjects.push_back(tempObject);
			return &engineObjects.back();
		}

		// Removes a game object with the given name
		void DeleteGameObject(std::string objectName)
		{
			for (int i = 0; i < engineObjects.size(); i++)
			{
				if (engineObjects[i].name == objectName)
				{
					engineObjects.erase(engineObjects.begin() + i);
					return;
				}
			}
			printf("Game object \"%s\" doesn't exist!\n", objectName.c_str());
			console::WaitFor(' ', true);
			exit(69);
		}

		// Returns a reference to a wanted game object
		gameobject* GetGameObject(std::string objectName)
		{
			for (int i = 0; i < engineObjects.size(); i++)
			{
				if (engineObjects[i].name == objectName)
				{
					return &engineObjects[i];
				}
			}
			printf("Game object \"%s\" doesn't exist!\n", objectName.c_str());
			console::WaitFor(' ', true);
			exit(69);
		}

		// Adds a new texture to the engine memory
		id NewTexture(bitmap& textureData)
		{
			tempTextureWidth = textureData.GetWidth();
			tempTextureHeight = textureData.GetHeight();
			tempTextureData = textureData.GetPixelData();
			createTexture = true;
			while (createTexture);
			return tempTextureID;
		}
		id NewTexture(bitmap&& textureData)
		{
			tempTextureWidth = textureData.GetWidth();
			tempTextureHeight = textureData.GetHeight();
			tempTextureData = textureData.GetPixelData();
			createTexture = true;
			while (createTexture);
			return tempTextureID;
		}

	private:
		// Misc
		bool engineRunning = false;
		window* engineWindow = NULL;
		int engineWidth = 0;
		int engineHeight = 0;

		// Time
		time engineTime = time();

		// Objects
		std::vector<gameobject> engineObjects = {};

		// Textures
		bool createTexture = false;
		int tempTextureWidth = 0;
		int tempTextureHeight = 0;
		color* tempTextureData = NULL;
		id tempTextureID = 0;

		// Computing object physics 
		void ObjectPhysics()
		{
			for (int i = 0; i < engineObjects.size(); i++)
			{
				if (engineObjects[i].physics)
				{
					// Applying gravity
					engineObjects[i].velocity.y -= gravity * engineObjects[i].gravityMulti * deltaTime;

					// Applying velocity
					engineObjects[i].position.x += engineObjects[i].velocity.x * deltaTime;
					engineObjects[i].position.y += engineObjects[i].velocity.y * deltaTime;
					engineObjects[i].position.z += engineObjects[i].velocity.z * deltaTime;

					// Applying angular momentum
					engineObjects[i].rotation.x += engineObjects[i].angularMo.x * deltaTime;
					engineObjects[i].rotation.y += engineObjects[i].angularMo.y * deltaTime;
					engineObjects[i].rotation.z += engineObjects[i].angularMo.z * deltaTime;
				}
			}
		}

		// Engine game loop
		void EngineLoop()
		{
			EngineStart();
			while (engineRunning && engineWindow->GetHWND())
			{
				/* Game input */
				EngineInput((char)engineWindow->KEY);

				/* Game logic */
				EngineUpdate();

				/* Applying physics */
				ObjectPhysics();

				// Delta time calculation
				deltaTime = engineTime.GetElapsed();

				// Displaying the FPS
				engineWindow->SetTitle(int(1 / engineWindow->deltaTime));
			}
		}
	};
}
