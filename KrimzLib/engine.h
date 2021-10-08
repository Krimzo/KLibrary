#pragma once
#include <functional>
#include "KrimzLib/time.h"


namespace kl
{
	class engine
	{
	public:
		void temp()
		{
			// Creating window class instance
			kl::window testWindow;

			// Defining opengl start function
			testWindow.OpenGLStart = [&]()
			{
				std::cout << glGetString(GL_VERSION) << std::endl;

				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				glFrustum(-1, 1, -1, 1, 1, 40);
			};

			kl::triangle tr1 = { {
				{-0.90, -0.90, -2, 0, 0, 0, kl::constant::colorBlack.ToDouble()},
				{ 0.00,  0.00, -2, 0, 0, 0, kl::constant::colorWhite.ToDouble()},
				{ 0.00,  0.90, -2, 0, 0, 0, kl::constant::colorBlack.ToDouble()}
			} };
			kl::triangle tr2 = { {
				{-0.90, -0.90, -2, 0, 0, 0, kl::constant::colorBlack.ToDouble()},
				{ 0.00,  0.00, -2, 0, 0, 0, kl::constant::colorWhite.ToDouble()},
				{ 0.90, -0.90, -2, 0, 0, 0, kl::constant::colorBlack.ToDouble()}
			} };
			kl::triangle tr3 = { {
				{ 0.90, -0.90, -2, 0, 0, 0, kl::constant::colorBlack.ToDouble()},
				{ 0.00,  0.00, -2, 0, 0, 0, kl::constant::colorWhite.ToDouble()},
				{ 0.00,  0.90, -2, 0, 0, 0, kl::constant::colorBlack.ToDouble()}
			} };

			kl::colord backgroundColor = kl::constant::colorDarkGray.ToDouble();
			double angle = 0;

			// Defining opengl update function
			testWindow.OpenGLUpdate = [&]()
			{
				kl::opengl::Clear(backgroundColor);

				angle += 36 * testWindow.deltaTime;

				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
				glRotated(angle, 0, 1, 0);

				kl::opengl::old::DrawTriangle(tr1);
				kl::opengl::old::DrawTriangle(tr2);
				kl::opengl::old::DrawTriangle(tr3);

				kl::opengl::FlipBuffers(testWindow.GetHDC());
				testWindow.SetTitle(int(1 / testWindow.deltaTime));
			};

			// Defining opengl quit function
			testWindow.OpenGLEnd = [&]()
			{

			};

			// Window creation and start
			testWindow.New(1100, 700, L"Testing Stuff", false, true);
		}



		// Engine properties
		double deltaTime = 0;

		// Outside functions that user defines
		std::function<void(void)> EngineStart = []() { };
		std::function<void(char)> EngineInput = [](char input) { };
		std::function<void(void)> EngineUpdate = []() { };

		// Starts the engine
		void Start()
		{
			engineRunning = true;
			EngineLoop();
		}

		// Stops the engine
		void Stop()
		{
			engineRunning = false;
		}

	private:
		// Private variables
		bool engineRunning = false;

		// Computing object physics 
		void ObjectPhysics()
		{

		}

		// Rendering objects to the screen
		void ObjectRender()
		{

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
				EngineInput("placeholder"[0]);

				/* Game logic */
				EngineUpdate();

				/* Applying physics */
				ObjectPhysics();

				/* Rendering */
				ObjectRender();

				/* Calculating frame time */
				//deltaTime = time::GetElapsed();

				/* Updating the title */
				// window.SetTitle(int(1 / deltaTime));
			}
		}
	};
}
