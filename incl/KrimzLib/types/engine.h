#pragma once


namespace kl {
	class engine {
	public:
		// Engine properties
		float deltaTime = 0;
		float elapsedTime = 0;
		float gravity = 9.81f;
		kl::color background = kl::color(50, 50, 50);
		kl::camera gameCamera = kl::camera();

		// User defined functions
		std::function<void()> start = []() {};
		std::function<void()> update = []() {};

		// Creates the engine
		void startNew(kl::size frameSize) {
			gameWindow.start = [&]() {
				/* Setting up the face culling */
				kl::opengl::setFaceCulling(false);

				/* Setting up the depth testing */
				kl::opengl::setDepthTest(true);

				/* Setting up the camera */
				gameCamera.setAspect(frameSize);
				gameCamera.setPlanes(0.01f, 100);
				gameCamera.sensitivity = 0.025f;

				/* Setting up the engine shaders */
				engineShaders = new kl::shaders(
					kl::file::readText("res/shaders/engine.vs"),
					kl::file::readText("res/shaders/engine.fs")
				);
				engineShaders->setUniform(engineShaders->getUniform("texture0"), 0);
				wvp = engineShaders->getUniform("wvp");

				/* Calling the user start */
				start();
			};

			gameWindow.update = [&]() {
				/* Clearing the buffers */
				kl::opengl::clearBuffers(background);

				/* Time calculations */
				elapsedTime = timer.stopwatchElapsed();
				deltaTime = timer.getElapsed();

				/* Calling the user update */
				update();

				/* Calling the physics update */
				computePhysics();

				/* Rendering */
				for (objItr = gObjects.begin(); objItr != gObjects.end(); objItr++) {
					if (objItr->visible) {
						engineShaders->setUniform(wvp, gameCamera.matrix() * objItr->geometry.matrix());
						objItr->render();
					}
				}

				/* Updating the fps display */
				gameWindow.setTitle(std::to_string(int(1 / deltaTime)));

				/* Swapping the frame buffers */
				gameWindow.swapFrameBuffers();
			};

			gameWindow.end = [&]() {
				delete engineShaders;
			};

			// Starting the window
			timer.getElapsed();
			timer.stopwatchReset();
			gameWindow.startNew(frameSize, kl::random::getString(6), false, true, true);
		}
		~engine() {
			stop();
		}
		void stop() {
			gameWindow.stop();
		}

		// Returns a reference to engine window
		kl::window& getWindow() {
			return gameWindow;
		}

		// Creates a new game object
		kl::gameobject* newObject() {
			gObjects.push_back(kl::gameobject());
			return &gObjects.back();
		}

		// Deletes a game object
		bool delObject(kl::gameobject* objectAddress) {
			for (objItr = gObjects.begin(); objItr != gObjects.end(); objItr++) {
				if (&*objItr == objectAddress) {
					gObjects.erase(objItr);
					return true;
				}
			}
			return false;
		}

	private:
		// Window data
		kl::time timer = kl::time();
		kl::window gameWindow = kl::window();

		// Shader data
		int wvp = NULL;
		kl::shaders* engineShaders = nullptr;

		// Object buffer
		std::list<kl::gameobject> gObjects = {};
		std::list<kl::gameobject>::iterator objItr = {};

		// Computing object physics 
		void computePhysics() {
			for (objItr = gObjects.begin(); objItr != gObjects.end(); objItr++) {
				if (objItr->physics.enabled) {
					// Applying gravity
					objItr->physics.velocity.y -= gravity * objItr->physics.gravity * deltaTime;

					// Applying velocity
					objItr->geometry.position.x += objItr->physics.velocity.x * deltaTime;
					objItr->geometry.position.y += objItr->physics.velocity.y * deltaTime;
					objItr->geometry.position.z += objItr->physics.velocity.z * deltaTime;

					// Applying angular momentum
					objItr->geometry.rotation.x += objItr->physics.angular.x * deltaTime;
					objItr->geometry.rotation.y += objItr->physics.angular.y * deltaTime;
					objItr->geometry.rotation.z += objItr->physics.angular.z * deltaTime;
				}
			}
		}
	};
}
