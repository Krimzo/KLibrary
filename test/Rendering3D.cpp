#include "KrimzLib.hpp"


// Rendering engine
kl::renderer3D renderer;

// Game objects
kl::object3D* wall = nullptr;
kl::object3D* table = nullptr;
kl::object3D* katanaL = nullptr;
kl::object3D* katanaR = nullptr;
kl::object3D* horse = nullptr;
kl::object3D* sphere1 = nullptr;
kl::object3D* metalcube1 = nullptr;
kl::object3D* metalcube2 = nullptr;
kl::object3D* kerv = nullptr;

// Renderer setup
void setup() {
	// Skybox creation
	renderer.newSkybox(
		"res/skyboxes/clouds/front.jpg",
		"res/skyboxes/clouds/back.jpg",
		"res/skyboxes/clouds/left.jpg",
		"res/skyboxes/clouds/right.jpg",
		"res/skyboxes/clouds/top.jpg",
		"res/skyboxes/clouds/bottom.jpg"
	);

	// Mesh creation
	kl::mesh* cube_mes = renderer.newMesh("res/objects/cube.obj");
	kl::mesh* table_mes = renderer.newMesh("res/objects/table.obj");
	kl::mesh* katana_mes = renderer.newMesh("res/objects/katana.obj");
	kl::mesh* horse_mes = renderer.newMesh("res/objects/horse.obj");
	kl::mesh* sphere_mes = renderer.newMesh("res/objects/sphere.obj");
	kl::mesh* tv_mes = renderer.newMesh("res/objects/tv.obj");

	// Texture creation
	kl::texture* solid1_tex = renderer.newTexture(kl::image(kl::ivec2(1, 1), kl::color(235, 180, 120)));
	kl::texture* solid2_tex = renderer.newTexture(kl::image(kl::ivec2(1, 1), kl::color(120, 180, 200)));
	kl::texture* table_tex = renderer.newTexture("res/textures/table.jpg");
	kl::texture* katana_tex = renderer.newTexture("res/textures/katana.jpg");
	kl::texture* horse_tex = renderer.newTexture("res/textures/horse.jpg");
	kl::texture* tv_tex = renderer.newTexture("res/textures/tv.jpg");
	kl::texture* peace_tex = renderer.newTexture("res/textures/peace.jpg");
	kl::texture* fp_tex = renderer.newTexture("res/textures/firepower.jpg");

	// Object creation
	wall = renderer.newObject(cube_mes, solid1_tex);
	table = renderer.newObject(table_mes, table_tex);
	katanaL = renderer.newObject(katana_mes, katana_tex);
	katanaR = renderer.newObject(katana_mes, katana_tex);
	horse = renderer.newObject(horse_mes, horse_tex);
	sphere1 = renderer.newObject(sphere_mes, solid2_tex);
	metalcube1 = renderer.newObject(cube_mes, peace_tex);
	metalcube2 = renderer.newObject(cube_mes, fp_tex);
	kerv = renderer.newObject(tv_mes, tv_tex);

	// Object properties setup
	wall->size = kl::vec3(50, 10, 0.05f);
	wall->position = kl::vec3(0, 0, -7);

	table->size = kl::vec3(1, 1, 1);
	table->rotation = kl::vec3(0, 45, 0);
	table->position = kl::vec3(0, -0.5, 2);

	katanaL->size = kl::vec3(2, 2, 2);
	katanaL->rotation = kl::vec3(0, 180, -42);
	katanaL->position = kl::vec3(-1, 0, 2);

	katanaR->size = kl::vec3(2, 2, 2);
	katanaR->rotation = kl::vec3(0, 0, 42);
	katanaR->position = kl::vec3(1, 0, 2);

	horse->size = kl::vec3(4, 4, 4);
	horse->position = kl::vec3(0, -0.02f, 2);
	horse->physics = true;
	horse->angular.y = 18;

	sphere1->size = kl::vec3(1, 1, 1);
	sphere1->position = kl::vec3(0, 2, -2);
	sphere1->physics = true;
	sphere1->velocity.x = 1;

	metalcube1->size = kl::vec3(0.5, 0.5, 0.5);
	metalcube1->rotation = kl::vec3(45, 45, 0);
	metalcube1->position = kl::vec3(0, 4, -2);
	metalcube1->physics = true;
	metalcube1->angular = kl::vec3(kl::random::FLOAT(-32, 32), kl::random::FLOAT(-32, 32), kl::random::FLOAT(-32, 32));

	metalcube2->size = kl::vec3(0.5, 0.5, 0.5);
	metalcube2->rotation = kl::vec3(45, 45, 0);
	metalcube2->position = kl::vec3(0, -4, -2);
	metalcube2->physics = true;
	metalcube2->angular = kl::vec3(kl::random::FLOAT(-32, 32), kl::random::FLOAT(-32, 32), kl::random::FLOAT(-32, 32));

	kerv->size = kl::vec3(5, 5, 5);
	kerv->rotation = kl::vec3(0, 180, 0);
	kerv->position = kl::vec3(0, 3, -6);

	// Sun setup
	renderer.sun.direction = kl::vec3(-0.575f, -0.75f, -2);

	// Going into fullscreen
	renderer.setFullscreen(true);
}

// Renderer input
bool movingCam = false;
void input(kl::keys* keys, kl::mouse* mouse) {
	// Keyboard input
	if (keys->w) {
		renderer.cam.moveForward(renderer.deltaT);
	}
	if (keys->s) {
		renderer.cam.moveBack(renderer.deltaT);
	}
	if (keys->d) {
		renderer.cam.moveRight(renderer.deltaT);
	}
	if (keys->a) {
		renderer.cam.moveLeft(renderer.deltaT);
	}
	if (keys->space) {
		renderer.cam.moveUp(renderer.deltaT);
	}
	if (keys->c) {
		renderer.cam.moveDown(renderer.deltaT);
	}
	if (keys->shift) {
		renderer.cam.speed = 5;
	}
	else {
		renderer.cam.speed = 2;
	}

	// Mouse input
	if (mouse->lmb) {
		movingCam = true;
		mouse->hide();

		// Fixing the camera jump on the first click
		mouse->position = renderer.frameCenter();
	}
	if (mouse->rmb) {
		movingCam = false;
		mouse->show();
	}
	if (movingCam) {
		kl::ivec2 frameCenter = renderer.frameCenter();
		renderer.cam.rotate(mouse->position, frameCenter);
		mouse->move(frameCenter);
	}

	// Other
	if (keys->esc) {
		renderer.stop();
	}
	if (keys->insert) {
		renderer.setFullscreen(true);
	}
	if (keys->delet) {
		renderer.setFullscreen(false);
	}
	if (keys->v) {
		kl::gl::setWireframe(true);
	}
	if (keys->f) {
		kl::gl::setWireframe(false);
	}
}

// Renderer update
void update() {
	const float horseGrav = 0.1f;
	sphere1->velocity += (horse->position - sphere1->position) * horseGrav * renderer.deltaT;

	const float r = 2;
	const float sinT = sin(renderer.elapsedT);
	const float cosT = cos(renderer.elapsedT);
	metalcube1->position.x = sphere1->position.x + r * sinT;
	metalcube1->position.y = sphere1->position.y + r * sinT;
	metalcube1->position.z = sphere1->position.z + r * cosT;
	metalcube2->position.x = sphere1->position.x - r * sinT;
	metalcube2->position.y = sphere1->position.y - r * sinT;
	metalcube2->position.z = sphere1->position.z - r * cosT;
}

int main() {
	// User start
	renderer.setup = setup;
	
	// User input
	renderer.input = input;

	// User update
	renderer.update = update;

	// Renderer creation
	renderer.createNew(kl::ivec2(1600, 900));


	return 0;
}