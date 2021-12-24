#include "KrimzLib.hpp"


int main() {
	/* Engine */
	kl::renderer renderer;

	/* Meshes */
	kl::mesh* table_mes = nullptr;
	kl::mesh* katana_mes = nullptr;
	kl::mesh* horse_mes = nullptr;
	kl::mesh* tv_mes = nullptr;

	/* Textures */
	kl::texture* table_tex = nullptr;
	kl::texture* katana_tex = nullptr;
	kl::texture* horse_tex = nullptr;
	kl::texture* tv_tex = nullptr;

	/* Objects */
	kl::renderable* table = nullptr;
	kl::renderable* katanaL = nullptr;
	kl::renderable* katanaR = nullptr;
	kl::renderable* horse = nullptr;
	kl::array<kl::renderable*> tvs(101);

	/* User start */
	renderer.setup = [&]() {
		/* Skybox creation */
		renderer.newSkybox(
			"res/skyboxes/clouds/front.jpg",
			"res/skyboxes/clouds/back.jpg",
			"res/skyboxes/clouds/left.jpg",
			"res/skyboxes/clouds/right.jpg",
			"res/skyboxes/clouds/top.jpg",
			"res/skyboxes/clouds/bottom.jpg"
		);

		/* Mesh creation */
		table_mes = renderer.newMesh("res/objects/table.obj");
		katana_mes = renderer.newMesh("res/objects/katana.obj");
		horse_mes = renderer.newMesh("res/objects/horse.obj");
		tv_mes = renderer.newMesh("res/objects/tv.obj");

		/* Texture creation */
		table_tex = renderer.newTexture("res/textures/table.jpg");
		katana_tex = renderer.newTexture("res/textures/katana.jpg");
		horse_tex = renderer.newTexture("res/textures/horse.jpg");
		tv_tex = renderer.newTexture("res/textures/tv.jpg");

		/* Object creation */
		table = renderer.newObject(table_mes, table_tex);
		katanaL = renderer.newObject(katana_mes, katana_tex);
		katanaR = renderer.newObject(katana_mes, katana_tex);
		horse = renderer.newObject(horse_mes, horse_tex);

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

		/* Object array creation */
		int tvCounter = 0;
		float tvsStartPos = -tvs.size() / 2.0;
		tvs.forEach([&](auto tv) {
			*tv = renderer.newObject(tv_mes, tv_tex);

			(*tv)->geometry.size = kl::vec3(2, 2, 2);
			(*tv)->geometry.rotation = kl::vec3(0, 180, 0);
			(*tv)->geometry.position = kl::vec3(tvsStartPos + tvCounter++, 0, -1);
		});
	};

	/* User update */
	bool camMoving = false;
	renderer.update = [&]() {
		/* Keyboard input */
		if (renderer.keys->w) {
			renderer.cam.moveForward(renderer.deltaT);
		}
		if (renderer.keys->s) {
			renderer.cam.moveBack(renderer.deltaT);
		}
		if (renderer.keys->d) {
			renderer.cam.moveRight(renderer.deltaT);
		}
		if (renderer.keys->a) {
			renderer.cam.moveLeft(renderer.deltaT);
		}
		if (renderer.keys->space) {
			renderer.cam.moveUp(renderer.deltaT);
		}
		if (renderer.keys->c) {
			renderer.cam.moveDown(renderer.deltaT);
		}
		if (renderer.keys->shift) {
			renderer.cam.speed = 5;
		}
		else {
			renderer.cam.speed = 2;
		}

		/* Mouse input */
		if (renderer.mouse->lmb) {
			camMoving = true;
			renderer.mouse->hide();

			// Fixing the camera jump on the first click
			renderer.mouse->position = renderer.frameCenter();
		}
		if (renderer.mouse->rmb) {
			camMoving = false;
			renderer.mouse->show();
		}
		if (camMoving) {
			kl::point frameCenter = renderer.frameCenter();
			renderer.cam.rotate(renderer.mouse->position, frameCenter);
			renderer.mouse->move(frameCenter);
		}

		/* Object updating */
		tvs.forEach([&](auto tv) {
			const float tvx = (*tv)->geometry.position.x;

			(*tv)->geometry.position.y = 1.5 * (sin((0.5 * tvx) + (3 * renderer.elapsedT)) + 1);
		});
	};

	/* Renderer creation */
	renderer.createNew(kl::size(1600, 900));


	return 0;
}