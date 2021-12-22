#include "KrimzLib.hpp"


int main() {
	/* Engine */
	kl::engine testEngine;

	/* Skybox */
	kl::skybox* cloudsBox = nullptr;

	/* Meshes */
	kl::mesh* tableMes = nullptr;
	kl::mesh* pyramidMes = nullptr;
	kl::mesh* katanaMes = nullptr;
	kl::mesh* horseMes = nullptr;

	/* Textures */
	kl::texture* tableTex = nullptr;
	kl::texture* checkersTex = nullptr;
	kl::texture* katanaTex = nullptr;
	kl::texture* horseTex = nullptr;

	/* Objects */
	kl::gameobject* table = nullptr;
	kl::gameobject* pyramid = nullptr;
	kl::gameobject* katanaL = nullptr;
	kl::gameobject* katanaR = nullptr;
	kl::gameobject* horse = nullptr;

	/* User start */
	testEngine.setup = [&]() {
		/* Skybox creation */
		cloudsBox = new kl::skybox(
			"res/skybox/front.jpg",
			"res/skybox/back.jpg",
			"res/skybox/left.jpg",
			"res/skybox/right.jpg",
			"res/skybox/top.jpg",
			"res/skybox/bottom.jpg"
		);
		testEngine.skybox = cloudsBox;

		/* Mesh creation */
		tableMes = new kl::mesh("res/objects/table.obj");
		pyramidMes = new kl::mesh("res/objects/pyramid.obj");
		katanaMes = new kl::mesh("res/objects/katana.obj");
		horseMes = new kl::mesh("res/objects/horse.obj");

		/* Texture creation */
		tableTex = new kl::texture("res/textures/table.png");
		checkersTex = new kl::texture("res/textures/checkers.png");
		katanaTex = new kl::texture("res/textures/katana.png");
		horseTex = new kl::texture("res/textures/horse.png");

		/* Object creation */
		table = testEngine.newObject();
		pyramid = testEngine.newObject();
		katanaL = testEngine.newObject();
		katanaR = testEngine.newObject();
		horse = testEngine.newObject();

		/* Mesh binding */
		table->mesh = tableMes;
		pyramid->mesh = pyramidMes;
		katanaL->mesh = katanaMes;
		katanaR->mesh = katanaMes;
		horse->mesh = horseMes;

		/* Texture binding */
		table->texture = tableTex;
		pyramid->texture = checkersTex;
		katanaL->texture = katanaTex;
		katanaR->texture = katanaTex;
		horse->texture = horseTex;

		/* Object properties setup */
		table->geometry.size = kl::vec3(1, 1, 1);
		table->geometry.rotation = kl::vec3(0, 0, 0);
		table->geometry.position = kl::vec3(0, -0.5, 2);
		table->physics.enabled = false;
		table->physics.angular.y = 18;
		table->physics.gravity = 0;

		pyramid->geometry.size = kl::vec3(0.35, 0.35, 0.35);
		pyramid->geometry.rotation = kl::vec3(0, 0, 0);
		pyramid->geometry.position = kl::vec3(0, 0.47, 2);
		pyramid->physics.enabled = true;
		pyramid->physics.angular.y = 18;
		pyramid->physics.gravity = 0;

		katanaL->geometry.size = kl::vec3(2, 2, 2);
		katanaL->geometry.rotation = kl::vec3(0, 180, -42);
		katanaL->geometry.position = kl::vec3(-1, 0, 2);
		katanaL->physics.enabled = false;
		katanaL->physics.angular.y = 36;
		katanaL->physics.gravity = 0;
		
		katanaR->geometry.size = kl::vec3(2, 2, 2);
		katanaR->geometry.rotation = kl::vec3(0, 0, 42);
		katanaR->geometry.position = kl::vec3(1, 0, 2);
		katanaR->physics.enabled = false;
		katanaR->physics.angular.y = -36;
		katanaR->physics.gravity = 0;

		horse->geometry.size = kl::vec3(2, 2, 2);
		horse->geometry.rotation = kl::vec3(0, 0, 0);
		horse->geometry.position = kl::vec3(0, 0.55, 2);
		horse->physics.enabled = true;
		horse->physics.angular.y = 18;
		horse->physics.gravity = 0;
	};

	/* User update */
	bool camMoving = false;
	testEngine.update = [&]() {
		/* Keyboard input */
		if (testEngine.keys->w) {
			testEngine.camera.moveForward(testEngine.delta);
		}
		if (testEngine.keys->s) {
			testEngine.camera.moveBack(testEngine.delta);
		}
		if (testEngine.keys->d) {
			testEngine.camera.moveRight(testEngine.delta);
		}
		if (testEngine.keys->a) {
			testEngine.camera.moveLeft(testEngine.delta);
		}
		if (testEngine.keys->space) {
			testEngine.camera.moveUp(testEngine.delta);
		}
		if (testEngine.keys->c) {
			testEngine.camera.moveDown(testEngine.delta);
		}
		if (testEngine.keys->shift) {
			testEngine.camera.speed = 5;
		}
		else {
			testEngine.camera.speed = 2;
		}

		/* Mouse input */
		if (testEngine.mouse->lmb) {
			camMoving = true;
			testEngine.mouse->hide();

			// Fixing the camera jump on the first click
			testEngine.mouse->position = testEngine.frameCenter();
		}
		if (testEngine.mouse->rmb) {
			camMoving = false;
			testEngine.mouse->show();
		}
		if (camMoving) {
			kl::point frameCenter = testEngine.frameCenter();
			testEngine.camera.rotate(testEngine.mouse->position, frameCenter);
			testEngine.mouse->move(frameCenter);
		}
	};

	/* Engine creation */
	testEngine.createNew(kl::size(1600, 900));

	/* Deleting skybox */
	delete cloudsBox;

	/* Deleting meshes */
	delete tableMes;
	delete pyramidMes;
	delete katanaMes;
	delete horseMes;

	/* Deleting textures */
	delete tableTex;
	delete checkersTex;
	delete katanaTex;
	delete horseTex;

	return 0;
}