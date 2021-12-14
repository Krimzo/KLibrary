#pragma once


namespace kl {
	class engine {
	public:
		// Engine properties
		float fpsLimit;
		float deltaTime;
		float gravity;
		kl::color background;
		kl::camera camera;

		// User defined functions
		std::function<void(void)> start;
		std::function<void(void)> update;

		// Constructor
		engine() {
			fpsLimit = -1;
			deltaTime = 0;
			gravity = 9.81f;
			background = kl::constant::colors::gray;
			camera = {};
			start = []() {};
			update = []() {};
		}

		// Creates the engine
		void startNew(kl::size size) {
			window.start = [&]() {

			};

			window.update = [&]() {

			};

			window.end = [&]() {

			};

			window.startNew(size, kl::random::getString(6), false, true, true);
		}
		void stop() {
			window.stop();
		}
		~engine() {
			this->stop();
		}

		// Creates a new game object
		kl::gameobject* addObject(kl::gameobject& gameObj) {
			gObjects.push_back(gameObj);
			return &gObjects.back();
		}

		// Deletes a game object
		bool deleteGameObject(kl::gameobject* objectAddress) {
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
		void physicsUpdate() {
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
