#pragma once
#include <functional>
#include "KrimzLib/KrimzTime.h"


namespace kl {
	class engine {
	public:
		// Engine properties
		double deltaTime = 0;

		// Outside functions that user defines
		std::function<void(void)> EngineStart = []() {};
		std::function<void(char)> EngineInput = [](char input) {};
		std::function<void(void)> EngineUpdate = []() {};

		// Starts the engine
		void Start() {
			engineRunning = true;
			EngineLoop();
		}

		// Stops the engine
		void Stop() {
			engineRunning = false;
		}

	private:
		// Private variables
		bool engineRunning = false;

		// Computing object physics 
		void ObjectPhysics() {

		}

		// Rendering objects to the screen
		void ObjectRender() {

		}

		// Engine game loop
		void EngineLoop() {
			// One call before engine start
			EngineStart();

			// Needed for time calculations
			while (engineRunning) {
				/* Game input */
				EngineInput("placeholder"[0]);

				/* Game logic */
				EngineUpdate();

				/* Applying physics */
				ObjectPhysics();

				/* Rendering */
				ObjectRender();

				/* Calculating frame time */
				deltaTime = time::GetElapsed();

				/* Updating the title */
				// window.SetTitle(int(1 / deltaTime));
			}
		}
	};
}
