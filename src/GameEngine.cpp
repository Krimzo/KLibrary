#include "KrimzLib.hpp"


int main() {
	/* Engine */
	kl::engine testEngine;

	/* Shaders */
	kl::shaders* basicShaders = nullptr;

	/* Textures */
	kl::texture* checkersTex = nullptr;
	kl::texture* tableTex = nullptr;
	kl::texture* katanaTex = nullptr;

	/* Objects */
	kl::gameobject* pyramid = nullptr;
	kl::gameobject* table = nullptr;
	kl::gameobject* katana = nullptr;
	kl::gameobject* katana2 = nullptr;

	/* User start */
	testEngine.start = [&]() {
		/* Shaders */
		basicShaders = new kl::shaders(kl::file::readText("res/shaders/basic.vs"), kl::file::readText("res/shaders/basic.fs"));
		basicShaders->setUniform(basicShaders->getUniform("texture0"), 0);
		testEngine.setShaders(basicShaders, "wvp");

		/* Textures */
		checkersTex = new kl::texture("res/textures/checkers.png");
		tableTex = new kl::texture("res/textures/table.png");
		katanaTex = new kl::texture("res/textures/katana.png");

		/* Object creation */
		pyramid = testEngine.newObject();
		table = testEngine.newObject();
		katana = testEngine.newObject();
		katana2 = testEngine.newObject();

		/* Loading vertex/index data */
		pyramid->loadData("res/objects/pyramid.obj");
		table->loadData("res/objects/table.obj");
		katana->loadData("res/objects/katana.obj");
		katana2->loadData("res/objects/katana.obj");

		/* Binding the textures */
		pyramid->setTexture(checkersTex);
		table->setTexture(tableTex);
		katana->setTexture(katanaTex);
		katana2->setTexture(katanaTex);

		/* Setting obj properties */
		pyramid->physics.enabled = true;
		pyramid->physics.angular.y = 36;
		pyramid->physics.gravity = 0;
		pyramid->geometry.position = kl::vec3(0, 0, 2);

		table->physics.enabled = true;
		table->physics.angular.y = -36;
		table->physics.gravity = 0;
		table->geometry.position = kl::vec3(0, -1, 2);

		katana->physics.enabled = true;
		katana->physics.angular.y = 36;
		katana->physics.gravity = 0;
		katana->geometry.position = kl::vec3(-2, 0, 2);

		katana2->physics.enabled = true;
		katana2->physics.angular.y = -36;
		katana2->physics.gravity = 0;
		katana2->geometry.position = kl::vec3(2, 0, 2);
	};

	/* User update */
	bool camMoving = false;
	testEngine.update = [&]() {
		/* Keyboard input */
		if (testEngine.getWindow().keys.w) {
			testEngine.gameCamera.moveForward(testEngine.deltaTime);
		}
		if (testEngine.getWindow().keys.s) {
			testEngine.gameCamera.moveBack(testEngine.deltaTime);
		}
		if (testEngine.getWindow().keys.d) {
			testEngine.gameCamera.moveRight(testEngine.deltaTime);
		}
		if (testEngine.getWindow().keys.a) {
			testEngine.gameCamera.moveLeft(testEngine.deltaTime);
		}
		if (testEngine.getWindow().keys.space) {
			testEngine.gameCamera.moveUp(testEngine.deltaTime);
		}
		if (testEngine.getWindow().keys.c) {
			testEngine.gameCamera.moveDown(testEngine.deltaTime);
		}
		if (testEngine.getWindow().keys.shift) {
			testEngine.gameCamera.speed = 5;
		}
		else {
			testEngine.gameCamera.speed = 2;
		}

		/* Mouse input */
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
			kl::point frameCenter = testEngine.getWindow().getCenter();
			testEngine.gameCamera.rotate(testEngine.getWindow().mouse.position, frameCenter);
			testEngine.getWindow().mouse.move(frameCenter);
		}
	};

	/* Window creation */
	testEngine.startNew(kl::size(1600, 900));

	/* Cleanup */
	delete basicShaders;
	delete checkersTex;
	delete tableTex;
	delete katanaTex;

	return 0;
}