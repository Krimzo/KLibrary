#include "KrimzLib.hpp"


// Rendering engine
kl::dx::renderer renderer;

// Game objects
kl::renderable* wall = nullptr;
kl::renderable* table = nullptr;
kl::renderable* katanaL = nullptr;
kl::renderable* katanaR = nullptr;
kl::renderable* horse = nullptr;
kl::renderable* sphere1 = nullptr;
kl::renderable* metalcube1 = nullptr;
kl::renderable* metalcube2 = nullptr;
kl::renderable* kerv = nullptr;

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
	kl::dx::mesh* cube_mes = renderer.newMesh("res/objects/cube.obj");
	kl::dx::mesh* table_mes = renderer.newMesh("res/objects/table.obj");
	kl::dx::mesh* katana_mes = renderer.newMesh("res/objects/katana.obj");
	kl::dx::mesh* horse_mes = renderer.newMesh("res/objects/horse.obj");
	kl::dx::mesh* sphere_mes = renderer.newMesh("res/objects/sphere.obj");
	kl::dx::mesh* tv_mes = renderer.newMesh("res/objects/tv.obj");

	// Texture creation
	kl::dx::texture* solid1_tex = renderer.newTexture(kl::image(kl::ivec2(1, 1), kl::color(235, 180, 120)));
	kl::dx::texture* solid2_tex = renderer.newTexture(kl::image(kl::ivec2(1, 1), kl::color(120, 180, 200)));
	kl::dx::texture* table_tex = renderer.newTexture("res/textures/table.jpg");
	kl::dx::texture* katana_tex = renderer.newTexture("res/textures/katana.jpg");
	kl::dx::texture* horse_tex = renderer.newTexture("res/textures/horse.jpg");
	kl::dx::texture* tv_tex = renderer.newTexture("res/textures/tv.jpg");
	kl::dx::texture* peace_tex = renderer.newTexture("res/textures/peace.jpg");
	kl::dx::texture* fp_tex = renderer.newTexture("res/textures/firepower.jpg");

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
	wall->geometry.size = kl::vec3(50, 10, 0.05);
	wall->geometry.position = kl::vec3(0, 0, -7);

	table->geometry.size = kl::vec3(1, 1, 1);
	table->geometry.rotation = kl::vec3(0, 45, 0);
	table->geometry.position = kl::vec3(0, -0.5, 2);

	katanaL->geometry.size = kl::vec3(2, 2, 2);
	katanaL->geometry.rotation = kl::vec3(0, 180, -42);
	katanaL->geometry.position = kl::vec3(-1, 0, 2);

	katanaR->geometry.size = kl::vec3(2, 2, 2);
	katanaR->geometry.rotation = kl::vec3(0, 0, 42);
	katanaR->geometry.position = kl::vec3(1, 0, 2);

	horse->geometry.size = kl::vec3(4, 4, 4);
	horse->geometry.position = kl::vec3(0, -0.02, 2);
	horse->physics.enabled = true;
	horse->physics.angular.y = 18;
	horse->physics.gravity = 0;

	sphere1->geometry.size = kl::vec3(1, 1, 1);
	sphere1->geometry.position = kl::vec3(0, 2, -2);
	sphere1->physics.enabled = true;
	sphere1->physics.velocity.x = 1;
	sphere1->physics.gravity = 0;

	metalcube1->geometry.size = kl::vec3(0.5, 0.5, 0.5);
	metalcube1->geometry.rotation = kl::vec3(45, 45, 0);
	metalcube1->geometry.position = kl::vec3(0, 4, -2);
	metalcube1->physics.enabled = true;
	metalcube1->physics.angular = kl::vec3(kl::random::INT(-32, 32), kl::random::INT(-32, 32), kl::random::INT(-32, 32));
	metalcube1->physics.gravity = 0;

	metalcube2->geometry.size = kl::vec3(0.5, 0.5, 0.5);
	metalcube2->geometry.rotation = kl::vec3(45, 45, 0);
	metalcube2->geometry.position = kl::vec3(0, -4, -2);
	metalcube2->physics.enabled = true;
	metalcube2->physics.angular = kl::vec3(kl::random::INT(-32, 32), kl::random::INT(-32, 32), kl::random::INT(-32, 32));
	metalcube2->physics.gravity = 0;

	kerv->geometry.size = kl::vec3(5, 5, 5);
	kerv->geometry.rotation = kl::vec3(0, 180, 0);
	kerv->geometry.position = kl::vec3(0, 3, -6);
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
	if (keys->v) {
		renderer.wireframe(true);
	}
	if (keys->f) {
		renderer.wireframe(false);
	}

	if (keys->i) {
		horse->geometry.position.z += renderer.cam.speed * renderer.deltaT;
	}
	if (keys->k) {
		horse->geometry.position.z -= renderer.cam.speed * renderer.deltaT;
	}
	if (keys->u) {
		horse->geometry.position.y += renderer.cam.speed * renderer.deltaT;
	}
	if (keys->o) {
		horse->geometry.position.y -= renderer.cam.speed * renderer.deltaT;
	}
}

// Renderer update
void update() {
	const float horseGrav = 0.1;
	sphere1->physics.velocity += (horse->geometry.position - sphere1->geometry.position) * horseGrav * renderer.deltaT;

	const float r = 2;
	const float sinT = sin(renderer.elapsedT);
	const float cosT = cos(renderer.elapsedT);
	metalcube1->geometry.position.x = sphere1->geometry.position.x + r * sinT;
	metalcube1->geometry.position.y = sphere1->geometry.position.y + r * sinT;
	metalcube1->geometry.position.z = sphere1->geometry.position.z + r * cosT;
	metalcube2->geometry.position.x = sphere1->geometry.position.x - r * sinT;
	metalcube2->geometry.position.y = sphere1->geometry.position.y - r * sinT;
	metalcube2->geometry.position.z = sphere1->geometry.position.z - r * cosT;
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