#pragma once
#include <functional>
#include "KrimzLib/time.h"


namespace kl
{
	class engine
	{
	public:
		// Engine properties
		double deltaTime = 0;

		// Outside functions that user defines
		std::function<void(void)> EngineStart = []() {};
		std::function<void(char)> EngineInput = [](char input) {};
		std::function<void(void)> EngineUpdate = []() {};

		// Creates the engine
		void Start(int width, int height, std::wstring name)
		{
			if (!engineRunning)
			{
				engineRunning = true;
				engineWindow = new window(width, height, name.c_str(), false, true);
				frameBuffer = bitmap(width, height);
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

	private:
		// Private variables
		bool engineRunning = false;
		window* engineWindow = NULL;
		bitmap frameBuffer = bitmap(0, 0);

		// Computing object physics 
		void ObjectPhysics()
		{

		}

		// Rendering objects to the screen
		void ObjectRender()
		{
			frameBuffer.FastClear(0);

			

			engineWindow->DisplayBitmap(frameBuffer);
		}

		// Engine game loop
		void EngineLoop()
		{
			// One call before engine start
			EngineStart();

			// Needed for time calculations
			while (engineRunning)
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
				deltaTime = engineWindow->deltaTime;

				/* Updating the title */
				engineWindow->SetTitle(int(1 / deltaTime));
			}
		}
	};
}
