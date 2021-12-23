#pragma once


namespace kl {
	struct renderer {
		// Input
		kl::keys* keys = nullptr;
		kl::mouse* mouse = nullptr;

		// Engine properties
		float delta = 0;
		float elapsed = 0;
		float gravity = 9.81;

		// View properties
		kl::skybox* sky = nullptr;
		kl::color background = kl::constant::colors::gray;
		kl::camera cam;
		
		// Ambient and directional lights
		kl::ambient dark;
		kl::direct sun;

		// User defined functions
		std::function<void()> setup = []() {};
		std::function<void()> update = []() {};

		// Starts a new engine
		void createNew(kl::size frameSize) {
			/* Engine timer */
			kl::time timer;

			/* Object shaders */
			kl::shaders* default_sha = nullptr;
			kl::uniform w_uni;
			kl::uniform vp_uni;
			kl::uniform dark_uni;
			kl::uniform sunL_uni;
			kl::uniform sunD_uni;

			/* Window start definition */
			win.start = [&]() {
				/* Setting up input */
				this->keys = &win.keys;
				this->mouse = &win.mouse;

				/* Setting up the face culling */
				kl::opengl::setCulling(false);

				/* Setting up the depth testing */
				kl::opengl::setDepthTest(true);

				/* Setting up the camera */
				cam.setAspect(frameSize);
				cam.setPlanes(0.01, 100);
				cam.sensitivity = 0.025;

				/* Setting up the lights */
				dark.color = kl::constant::colors::white;
				dark.intensity = 0.1;
				sun.color = kl::constant::colors::white;
				sun.intensity = 1;
				sun.direction = kl::vec3(-0.2, -0.2, -1);

				/* Compiling object shaders */
				default_sha = new kl::shaders(
					kl::file::readText("res/shaders/renderer.vert"),
					kl::file::readText("res/shaders/renderer.frag")
				);

				/* Getting object shader uniforms */
				w_uni = default_sha->getUniform("w");
				vp_uni = default_sha->getUniform("vp");
				dark_uni = default_sha->getUniform("dark");
				sunL_uni = default_sha->getUniform("sunL");
				sunD_uni = default_sha->getUniform("sunD");

				/* Calling the user start */
				setup();
			};

			/* Window update definition */
			win.update = [&]() {
				/* Clearing the buffers */
				kl::opengl::clearBuffers(background);

				/* Time calculations */
				delta = timer.getElapsed();
				elapsed = timer.stopwatchElapsed();

				/* Calling the user update */
				update();

				/* Calling the physics update */
				updatePhysics();

				/* Setting the camera uniforms */
				vp_uni.setData(cam.matrix());

				/* Setting the light uniforms */
				dark_uni.setData(dark.getColor());
				sunL_uni.setData(sun.getColor());
				sunD_uni.setData(sun.getDirection());

				/* Rendering objects */
				for (objItr = gObjects.begin(); objItr != gObjects.end(); objItr++) {
					if (objItr->visible) {
						w_uni.setData(objItr->geometry.matrix());
						objItr->render();
					}
				}

				/* Rendering skybox */
				if (sky) sky->render(cam.matrix());

				/* Updating the fps display */
				win.setTitle(std::to_string(int(1 / delta)));

				/* Swapping the frame buffers */
				win.swapFrameBuffers();
			};

			/* Window end definition */
			win.end = [&]() {
				delete default_sha;
			};

			/* Window creation */
			timer.getElapsed();
			timer.stopwatchReset();
			win.startNew(frameSize, kl::random::getString(6), false, true, true);
		}
		void stop() {
			win.stop();
		}

		// Returns the frame center
		kl::point frameCenter() {
			return win.getCenter();
		}

		// Creates a new game object
		kl::renderable* newObject() {
			gObjects.push_back(kl::renderable());
			return &gObjects.back();
		}

		// Deletes a game object
		bool delObject(kl::renderable* objectAddress) {
			for (objItr = gObjects.begin(); objItr != gObjects.end(); objItr++) {
				if (&*objItr == objectAddress) {
					gObjects.erase(objItr);
					return true;
				}
			}
			return false;
		}

	private:
		// Window
		kl::window win;

		// Object(renderable) buffer
		std::list<kl::renderable> gObjects;
		std::list<kl::renderable>::iterator objItr;

		// Computing object physics 
		void updatePhysics() {
			for (objItr = gObjects.begin(); objItr != gObjects.end(); objItr++) {
				if (objItr->physics.enabled) {
					// Applying gravity
					objItr->physics.velocity.y -= gravity * objItr->physics.gravity * delta;

					// Applying velocity
					objItr->geometry.position.x += objItr->physics.velocity.x * delta;
					objItr->geometry.position.y += objItr->physics.velocity.y * delta;
					objItr->geometry.position.z += objItr->physics.velocity.z * delta;

					// Applying angular momentum
					objItr->geometry.rotation.x += objItr->physics.angular.x * delta;
					objItr->geometry.rotation.y += objItr->physics.angular.y * delta;
					objItr->geometry.rotation.z += objItr->physics.angular.z * delta;
				}
			}
		}
	};
}
