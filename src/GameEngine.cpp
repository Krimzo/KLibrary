#include "KrimzLib.hpp"


int main() {
	// Engine
	kl::engine testEngine;
	
	// Data
	kl::shaders* basicShaders = nullptr;
	kl::texture* dogoTexture = nullptr;
	kl::gameobject* dogoCube = nullptr;

	// User start
	testEngine.start = [&]() {
		// Compiling shaders
		basicShaders = new kl::shaders(kl::file::readText("res/shaders/basic.vs"), kl::file::readText("res/shaders/basic.fs"));
		basicShaders->setUniform(basicShaders->getUniform("texture0"), 0);

		// Loading the textures
		dogoTexture = new kl::texture("res/textures/dogo.png");

		// Creating a game object
		dogoCube = testEngine.newObject();

		// Binding the shaders
		dogoCube->setShaders(basicShaders, "wvp");

		// Binding the texture
		dogoCube->setTexture(dogoTexture);

		// Binding the vertex and index data
		std::vector<kl::vertex> vertexData = {
			kl::vertex(kl::vec3(0.5,  0.5,  0.5), kl::vec2(1, 1), kl::random::getColor()),
			kl::vertex(kl::vec3(-0.5,  0.5,  0.5), kl::vec2(0, 1), kl::random::getColor()),
			kl::vertex(kl::vec3(0.5, -0.5,  0.5), kl::vec2(1, 0), kl::random::getColor()),
			kl::vertex(kl::vec3(-0.5, -0.5,  0.5), kl::vec2(0, 0), kl::random::getColor()),
			kl::vertex(kl::vec3(0.5,  0.5, -0.5), kl::vec2(1, 1), kl::random::getColor()),
			kl::vertex(kl::vec3(-0.5,  0.5, -0.5), kl::vec2(0, 1), kl::random::getColor()),
			kl::vertex(kl::vec3(0.5, -0.5, -0.5), kl::vec2(1, 0), kl::random::getColor()),
			kl::vertex(kl::vec3(-0.5, -0.5, -0.5), kl::vec2(0, 0), kl::random::getColor())
		};
		std::vector<kl::index> indexData = {
			0, 1, 3,
			0, 3, 2,
			0, 4, 5,
			0, 5, 1,
			0, 2, 6,
			0, 6, 4,
			7, 3, 1,
			7, 1, 5,
			7, 4, 6,
			7, 5, 4,
			7, 6, 2,
			7, 2, 3
		};
		dogoCube->setData(vertexData, indexData);
	};

	// User update
	testEngine.update = [&]() {
		/* Keyboard input */
		if (testEngine.getWindow().keys.w) {
			testEngine.camera.moveForward(testEngine.deltaTime);
		}
		if (testEngine.getWindow().keys.s) {
			testEngine.camera.moveBack(testEngine.deltaTime);
		}
		if (testEngine.getWindow().keys.d) {
			testEngine.camera.moveRight(testEngine.deltaTime);
		}
		if (testEngine.getWindow().keys.a) {
			testEngine.camera.moveLeft(testEngine.deltaTime);
		}
		if (testEngine.getWindow().keys.space) {
			testEngine.camera.moveUp(testEngine.deltaTime);
		}
		if (testEngine.getWindow().keys.c) {
			testEngine.camera.moveDown(testEngine.deltaTime);
		}
		if (testEngine.getWindow().keys.shift) {
			testEngine.camera.speed = 5;
		}
		else {
			testEngine.camera.speed = 2;
		}

		/* Mouse input */
		static bool camMoving = false;
		if (testEngine.getWindow().mouse.lmb) {
			camMoving = true;
			testEngine.getWindow().mouse.hide();

			// Fixing the camera jump on the first click
			testEngine.getWindow().mouse.position = testEngine.getWindow().getCenter();
		}
		if (testEngine.getWindow().mouse.rmb) {
			camMoving = false;
			testEngine.getWindow().mouse.show();
		}
		if (camMoving) {
			const kl::point frameCenter = testEngine.getWindow().getCenter();
			testEngine.camera.rotate(testEngine.getWindow().mouse.position, frameCenter);
			testEngine.getWindow().mouse.move(frameCenter);
		}
	};

	// Window creation
	testEngine.startNew(kl::size(1600, 900));

	// Cleanup
	delete basicShaders;
	delete dogoTexture;
	delete dogoCube;

	return 0;
}