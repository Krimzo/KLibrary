#include "KrimzLib.hpp"


int main() {
	/* Engine */
	kl::engine testEngine;

	/* Textures */
	kl::texture* tableTex = nullptr;
	kl::texture* checkersTex = nullptr;
	kl::texture* katanaTex = nullptr;

	/* Meshes */
	kl::mesh* tableMes = nullptr;
	kl::mesh* pyramidMes = nullptr;
	kl::mesh* katanaMes = nullptr;

	/* Objects */
	kl::gameobject* table = nullptr;
	kl::gameobject* pyramid = nullptr;
	kl::gameobject* katanaL = nullptr;
	kl::gameobject* katanaR = nullptr;

	/* User start */
	testEngine.start = [&]() {
		/* Mesh creation */
		tableMes = new kl::mesh("res/objects/table.obj");
		pyramidMes = new kl::mesh("res/objects/pyramid.obj");
		katanaMes = new kl::mesh("res/objects/katana.obj");

		/* Texture creation */
		tableTex = new kl::texture("res/textures/table.png", true);
		checkersTex = new kl::texture("res/textures/checkers.png", true);
		katanaTex = new kl::texture("res/textures/katana.png", true);

		/* Object creation */
		table = testEngine.newObject();
		pyramid = testEngine.newObject();
		katanaL = testEngine.newObject();
		katanaR = testEngine.newObject();

		/* Mesh binding */
		table->mesh = tableMes;
		pyramid->mesh = pyramidMes;
		katanaL->mesh = katanaMes;
		katanaR->mesh = katanaMes;

		/* Texture binding */
		table->texture = tableTex;
		pyramid->texture = checkersTex;
		katanaL->texture = katanaTex;
		katanaR->texture = katanaTex;

		/* Object properties setup */
		table->geometry.size = kl::vec3(1, 1, 1);
		table->geometry.position = kl::vec3(0, -0.5, 2);
		table->physics.enabled = true;
		table->physics.angular.y = 18;
		table->physics.gravity = 0;

		pyramid->geometry.size = kl::vec3(0.35, 0.35, 0.35);
		pyramid->geometry.position = kl::vec3(0, 0.47, 2);
		pyramid->visible = true;
		pyramid->physics.enabled = true;
		pyramid->physics.angular.y = 18;
		pyramid->physics.gravity = 0;

		katanaL->geometry.size = kl::vec3(2, 2, 2);
		katanaL->geometry.rotation = kl::vec3(0, 180, -45);
		katanaL->geometry.position = kl::vec3(-1, 0, 2);
		katanaL->physics.enabled = false;
		katanaL->physics.angular.y = 36;
		katanaL->physics.gravity = 0;
		
		katanaR->geometry.size = kl::vec3(2, 2, 2);
		katanaR->geometry.rotation = kl::vec3(0, 0, 45);
		katanaR->geometry.position = kl::vec3(1, 0, 2);
		katanaR->physics.enabled = false;
		katanaR->physics.angular.y = -36;
		katanaR->physics.gravity = 0;
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

	/* Engine creation */
	testEngine.startNew(kl::size(1600, 900));

	/* Cleanup */
	delete tableTex;
	delete checkersTex;
	delete katanaTex;
	delete tableMes;
	delete pyramidMes;
	delete katanaMes;

	return 0;
}