#include "KrimzLib.hpp"


int main() {
	/* Engine */
	kl::renderer testRenderer;

	/* Skybox */
	kl::skybox* clouds = nullptr;

	/* Meshes */
	kl::mesh* tableMes = nullptr;
	kl::mesh* katanaMes = nullptr;
	kl::mesh* horseMes = nullptr;

	/* Textures */
	kl::texture* tableTex = nullptr;
	kl::texture* katanaTex = nullptr;
	kl::texture* horseTex = nullptr;

	/* Objects */
	kl::renderable* table = nullptr;
	kl::renderable* katanaL = nullptr;
	kl::renderable* katanaR = nullptr;
	kl::renderable* horse = nullptr;

	/* User start */
	testRenderer.setup = [&]() {
		/* Skybox creation */
		clouds = new kl::skybox(
			"res/skybox/front.jpg",
			"res/skybox/back.jpg",
			"res/skybox/left.jpg",
			"res/skybox/right.jpg",
			"res/skybox/top.jpg",
			"res/skybox/bottom.jpg"
		);
		testRenderer.sky = clouds;

		/* Mesh creation */
		tableMes = new kl::mesh("res/objects/table.obj");
		katanaMes = new kl::mesh("res/objects/katana.obj");
		horseMes = new kl::mesh("res/objects/horse.obj");

		/* Texture creation */
		tableTex = new kl::texture("res/textures/table.png");
		katanaTex = new kl::texture("res/textures/katana.png");
		horseTex = new kl::texture("res/textures/horse.png");

		/* Object creation */
		table = testRenderer.newObject();
		katanaL = testRenderer.newObject();
		katanaR = testRenderer.newObject();
		horse = testRenderer.newObject();

		/* Mesh binding */
		table->mesh = tableMes;
		katanaL->mesh = katanaMes;
		katanaR->mesh = katanaMes;
		horse->mesh = horseMes;

		/* Texture binding */
		table->texture = tableTex;
		katanaL->texture = katanaTex;
		katanaR->texture = katanaTex;
		horse->texture = horseTex;

		/* Object properties setup */
		table->geometry.size = kl::vec3(1, 1, 1);
		table->geometry.rotation = kl::vec3(0, 45, 0);
		table->geometry.position = kl::vec3(0, -0.5, 2);
		table->physics.enabled = false;
		table->physics.angular.y = 18;
		table->physics.gravity = 0;

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

		horse->geometry.size = kl::vec3(4, 4, 4);
		horse->geometry.rotation = kl::vec3(0, 0, 0);
		horse->geometry.position = kl::vec3(0, -0.02, 2);
		horse->physics.enabled = true;
		horse->physics.angular.y = 18;
		horse->physics.gravity = 0;
	};

	/* User update */
	bool camMoving = false;
	testRenderer.update = [&]() {
		/* Keyboard input */
		if (testRenderer.keys->w) {
			testRenderer.camera.moveForward(testRenderer.delta);
		}
		if (testRenderer.keys->s) {
			testRenderer.camera.moveBack(testRenderer.delta);
		}
		if (testRenderer.keys->d) {
			testRenderer.camera.moveRight(testRenderer.delta);
		}
		if (testRenderer.keys->a) {
			testRenderer.camera.moveLeft(testRenderer.delta);
		}
		if (testRenderer.keys->space) {
			testRenderer.camera.moveUp(testRenderer.delta);
		}
		if (testRenderer.keys->c) {
			testRenderer.camera.moveDown(testRenderer.delta);
		}
		if (testRenderer.keys->shift) {
			testRenderer.camera.speed = 5;
		}
		else {
			testRenderer.camera.speed = 2;
		}

		/* Mouse input */
		if (testRenderer.mouse->lmb) {
			camMoving = true;
			testRenderer.mouse->hide();

			// Fixing the camera jump on the first click
			testRenderer.mouse->position = testRenderer.frameCenter();
		}
		if (testRenderer.mouse->rmb) {
			camMoving = false;
			testRenderer.mouse->show();
		}
		if (camMoving) {
			kl::point frameCenter = testRenderer.frameCenter();
			testRenderer.camera.rotate(testRenderer.mouse->position, frameCenter);
			testRenderer.mouse->move(frameCenter);
		}
	};

	/* Renderer creation */
	testRenderer.createNew(kl::size(1600, 900));

	/* Deleting skybox */
	delete clouds;

	/* Deleting meshes */
	delete tableMes;
	delete katanaMes;
	delete horseMes;

	/* Deleting textures */
	delete tableTex;
	delete katanaTex;
	delete horseTex;

	return 0;
}