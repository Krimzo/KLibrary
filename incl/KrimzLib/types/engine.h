#pragma once


namespace kl {
	class engine {
	public:
		// Engine properties
		float deltaTime;
		float elapsedTime;
		float gravity;
		kl::color background;
		kl::camera gameCamera;

		// User defined functions
		std::function<void()> start;
		std::function<void()> update;

		// Constructor
		engine() {
			deltaTime = 0;
			gravity = 9.81f;
			background = kl::constant::colors::gray;
			gameCamera = {};
			start = []() {};
			update = []() {};
		}

		// Creates the engine
		void startNew(kl::size frameSize) {
			gameWindow.start = [&]() {
				/* Setting the face culling */
				kl::opengl::setFaceCulling(false);

				/* Setting the depth testing */
				kl::opengl::setDepthTest(true);

				/* Setting up the camera */
				gameCamera.setAspect(frameSize);
				gameCamera.setPlanes(0.01f, 100);
				gameCamera.sensitivity = 0.025f;

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
				physics();

				/* Rendering */
				kl::mat4 cameraMat = gameCamera.matrix();
				for (objItr = gObjects.begin(); objItr != gObjects.end(); objItr++) {
					objItr->render(cameraMat);
				}

				/* Updating the fps display */
				gameWindow.setTitle(std::to_string(int(1 / deltaTime)));

				/* Swapping the frame buffers */
				gameWindow.swapFrameBuffers();
			};

			// Starting the window
			timer.getElapsed();
			timer.stopwatchReset();
			gameWindow.startNew(frameSize, kl::random::getString(6), false, true, true);
		}
		void stop() {
			gameWindow.stop();
		}
		~engine() {
			this->stop();
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
		kl::time timer = {};
		kl::window gameWindow = {};
		std::list<kl::gameobject> gObjects = {};
		std::list<kl::gameobject>::iterator objItr = {};

		// Computing object physics 
		void physics() {
			for (objItr = gObjects.begin(); objItr != gObjects.end(); objItr++) {
				if (objItr->physics) {
					// Applying gravity
					objItr->velocity.y -= gravity * objItr->gravity * deltaTime;

					// Applying velocity
					objItr->position.x += objItr->velocity.x * deltaTime;
					objItr->position.y += objItr->velocity.y * deltaTime;
					objItr->position.z += objItr->velocity.z * deltaTime;

					// Applying angular momentum
					objItr->rotation.x += objItr->angular.x * deltaTime;
					objItr->rotation.y += objItr->angular.y * deltaTime;
					objItr->rotation.z += objItr->angular.z * deltaTime;
				}
			}
		}
	};
}
