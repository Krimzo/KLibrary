#pragma once
#include <functional>
#include <windows.h>
#include "KrimzLib/window.h"
#include "KrimzLib/time.h"
#include "KrimzLib/color.h"
#include "KrimzLib/console.h"
#include "KrimzLib/constant.h"
#include "KrimzLib/math.h"


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
		double fov = 60;
		double windowDistance = 0;

		void CalculateWindowDistance(int frameWidth)
		{
			windowDistance = (frameWidth / 2.0) / tan((fov / 2.0) * kl::constant::toRadians);
		}
	};

	struct spotlight {
		color color = {};
		vec3 position = {};
		vec3 rotation = {};
		double fov = 60;
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
		bool useGpu = false;
		color ambientLight = { 255, 255, 255 };
		spotlight spotLight = {};
		byte background = 5;

		// Outside functions that user defines
		std::function<void(void)> EngineStart = []() {};
		std::function<void(char)> EngineInput = [](char input) {};
		std::function<void(void)> EngineUpdate = []() {};

		// Creates the engine
		void Start(int width, int height, std::wstring name)
		{
			if (!engineRunning)
			{
				// Misc
				engineRunning = true;
				engineWindow = new window(width, height, name.c_str(), false, true);
				engineWidth = engineWindow->GetWidth();
				engineHeight = engineWindow->GetHeight();
				engineCamera.CalculateWindowDistance(engineWidth);

				// Buffers
				frameBuffer = bitmap(width, height);
				depthBuffer.resize((size_t)width * (size_t)height);
				
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
			gameobject tempObject = { objectName };
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

	private:
		// Misc
		bool engineRunning = false;
		window* engineWindow = NULL;
		int engineWidth = 0;
		int engineHeight = 0;
		camera engineCamera = {};

		// Time
		time engineTime = kl::time();

		// Buffers
		bitmap frameBuffer = bitmap(0, 0);
		std::vector<double> depthBuffer = {};

		// Objects
		std::vector<gameobject> engineObjects = {};

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

		// Checks and updates depth buffer values if necessary
		bool CheckDepthBuffer(int x, int y, double z)
		{
			int location = y * engineWidth + x;
			if (location >= 0 && location < (engineWidth * engineHeight) && (!depthBuffer[location] || depthBuffer[location] > z))
			{
				depthBuffer[location] = z;
				return true;
			}
			return false;
		}

		// Rendering triangle with CPU
		void TriangleRenderCPU(triangle& tr)
		{
			int maxX = (int)min(max(tr.vertices[0].x, max(tr.vertices[1].x, tr.vertices[2].x)), engineWidth - 1.0);
			int minX = (int)max(min(tr.vertices[0].x, min(tr.vertices[1].x, tr.vertices[2].x)), 0.0f);
			int maxY = (int)min(max(tr.vertices[0].y, max(tr.vertices[1].y, tr.vertices[2].y)), engineHeight - 1.0);
			int minY = (int)max(min(tr.vertices[0].y, min(tr.vertices[1].y, tr.vertices[2].y)), 0.0f);

			//thread::ParallelFor(minY, maxY + 1, 4, [&](int y)
			for(int y = minY; y <= maxY; y++)
			{
				for (int x = minX; x <= maxX; x++)
				{
					if (tr.ContainsPoint(x, y))
					{
						// Calculate 3 bary ratios and world space z
						vec3 baryRatios = math::CalculateBarycentricRatios(tr, x, y);
						double pointZ = tr.vertices[0].z * baryRatios.x + tr.vertices[1].z * baryRatios.y + tr.vertices[2].z * baryRatios.z;

						// Check if the current point is inside the triangle
						if (pointZ > 0 && CheckDepthBuffer(x, y, pointZ))
						{
							// Calculating u, v and w
							double pointU = tr.vertices[0].u * baryRatios.x + tr.vertices[1].u * baryRatios.y + tr.vertices[2].u * baryRatios.z;
							double pointV = tr.vertices[0].v * baryRatios.x + tr.vertices[1].v * baryRatios.y + tr.vertices[2].v * baryRatios.z;
							double pointW = tr.vertices[0].w * baryRatios.x + tr.vertices[1].w * baryRatios.y + tr.vertices[2].w * baryRatios.z;
							pointU /= pointW;
							pointV /= pointW;

							// Getting the correct texture pixel
							color pixelColor = tr.texture->GetPixel(int(pointU * tr.texture->GetWidth()), int(pointV * tr.texture->GetHeight()));

							// Apply ambient light
							pixelColor.r = min(pixelColor.r * ambientLight.r / 255, 255);
							pixelColor.g = min(pixelColor.g * ambientLight.g / 255, 255);
							pixelColor.b = min(pixelColor.b * ambientLight.b / 255, 255);

							// Writing pixel in the memory
							frameBuffer.SetPixel(x, y, pixelColor);
						}
					}
				}
			}
		}

		// Rendering triangle with GPU
		void TriangleRenderGPU(triangle& t)
		{

		}

		// Render objects to the screen
		void ObjectRender()
		{
			// Clear buffers
			frameBuffer.FastClear(background);
			memset(&depthBuffer[0], 0, depthBuffer.size() * 8);

			// Render objects to the frame
			for (int i = 0; i < engineObjects.size(); i++)
			{
				if (engineObjects[i].visible)
				{
					for (int j = 0; j < engineObjects[i].triangles.size(); j++)
					{
						triangle cameraTriangle = engineObjects[i].triangles[j];

						// Compute triangle vertices
						for (int k = 0; k < 3; k++)
						{
							// Resize vertex
							cameraTriangle.vertices[k].Resize(engineObjects[i].size);

							// Apply vertex rotation
							cameraTriangle.vertices[k].Rotate(engineObjects[i].rotation);

							// Apply vertex translation
							cameraTriangle.vertices[k].Translate(engineObjects[i].position);

							// Apply camera translation and rotation
							cameraTriangle.vertices[k].Translate(engineCamera.position * -1);
							cameraTriangle.vertices[k].Rotate(engineCamera.rotation * -1);

							// Change the perspective from 3D space to 2D space
							cameraTriangle.vertices[k].ApplyPerspective(engineCamera.windowDistance / abs(cameraTriangle.vertices[k].z));

							// Center the points on screen
							cameraTriangle.vertices[k].ScreenFix(engineWidth, engineHeight);
						}

						// Rendering triangle
						if (useGpu)
						{
							TriangleRenderGPU(cameraTriangle);
						}
						else
						{
							TriangleRenderCPU(cameraTriangle);
						}
					}
				}
			}
			
			// Display the frame
			engineWindow->DisplayBitmap(frameBuffer);
		}

		// Engine game loop
		void EngineLoop()
		{
			// One call before engine start
			EngineStart();

			// Needed for time calculations
			while (engineRunning && engineWindow->GetHWND())
			{
				/* Game input */
				EngineInput((char)engineWindow->KEY);

				/* Game logic */
				EngineUpdate();

				/* Applying physics */
				ObjectPhysics();

				/* Rendering */
				ObjectRender();

				/* Calculating frame time */
				deltaTime = engineTime.GetElapsed();

				/* Updating the title */
				engineWindow->SetTitle(int(1 / deltaTime));
			}
		}
	};
}
