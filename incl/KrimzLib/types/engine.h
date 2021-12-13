#pragma once


namespace kl {
	class engine {
	public:
		// Engine properties
		float fpsLimit = -1;
		float deltaTime = 0;
		float gravity = 9.81f;
		kl::color background = {};
		kl::camera engineCamera = {};

		// User defined functions
		std::function<void(void)> engineStart = []() {};
		std::function<void(void)> engineUpdate = []() {};

		// Constructor
		engine() {
			fpsLimit = -1;
			deltaTime = 0;
			gravity = 9.81f;
			background = kl::constant::colors::gray;
			engineCamera = {};
		}

		// Creates the engine
		void startNew(kl::size size) {
			engineWindow.windowStart = [&]() {

			};

			engineWindow.windowUpdate = [&]() {

			};

			engineWindow.windowEnd = [&]() {

			};

			engineWindow.startNew(size, kl::random::getString(6), false, true, true);
		}
		void stop() {
			engineWindow.stop();
		}
		~engine() {
			this->stop();
		}

		// Creates a new game object
		kl::gameobject* addObject() {
			engineObjects.push_back(kl::gameobject());
			return &engineObjects.back();
		}
		kl::gameobject* addObject(kl::texture textureID) {
			engineObjects.push_back(kl::gameobject(textureID));
			return &engineObjects.back();
		}

		// Deletes a game object
		bool deleteGameObject(kl::gameobject* objectAddress) {
			for (objItr = engineObjects.begin(); objItr != engineObjects.end(); objItr++) {
				if (&*objItr == objectAddress) {
					engineObjects.erase(objItr);
					return true;
				}
			}
			return false;
		}

	private:
		kl::window engineWindow = {};
		kl::time engineTime = {};
		std::list<kl::gameobject> engineObjects = {};
		std::list<kl::gameobject>::iterator objItr = {};

		// Computing object physics 
		void physicsUpdate() {
			for (objItr = engineObjects.begin(); objItr != engineObjects.end(); objItr++) {
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
