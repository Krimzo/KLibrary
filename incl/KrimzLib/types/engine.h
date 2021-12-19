#pragma once


namespace kl {
	class engine {
	public:
		// Engine properties
		float deltaTime;
		float elapsedTime;
		float gravity;
		kl::color background;
		kl::camera camera;

		// User defined functions
		std::function<void()> start;
		std::function<void()> update;

		// Constructor
		engine() {
			deltaTime = 0;
			gravity = 9.81f;
			background = kl::constant::colors::gray;
			camera = {};
			start = []() {};
			update = []() {};
		}

		// Creates the engine
		void startNew(kl::size frameSize) {
			window.start = [&]() {
				/* Setting the face culling */
				kl::opengl::setFaceCulling(false);

				/* Setting the depth testing */
				kl::opengl::setDepthTest(true);

				/* Setting up the camera */
				camera.setAspect(frameSize);
				camera.setPlanes(0.01f, 100);
				camera.sensitivity = 0.025f;

				/* Calling the user start */
				start();
			};

			window.update = [&]() {
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
				for (objItr = gObjects.begin(); objItr != gObjects.end(); objItr++) {
					objItr->render();
				}

				/* Updating the fps display */
				window.setTitle(std::to_string(int(1 / deltaTime)));

				/* Swapping the frame buffers */
				window.swapFrameBuffers();
			};

			// Starting the window
			timer.getElapsed();
			timer.stopwatchReset();
			window.startNew(frameSize, kl::random::getString(6), false, true, true);
		}
		void stop() {
			window.stop();
		}
		~engine() {
			this->stop();
		}

		// Returns a reference to engine window
		kl::window& getWindow() {
			return window;
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
		kl::window window = {};
		kl::time timer = {};
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
