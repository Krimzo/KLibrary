#include "KrimzLib.hpp"


// Rendering engine
kl::gl::renderer renderer;

// Game objects
kl::gl::renderable* wall = nullptr;
kl::gl::renderable* table = nullptr;
kl::gl::renderable* katanaL = nullptr;
kl::gl::renderable* katanaR = nullptr;
kl::gl::renderable* horse = nullptr;
kl::gl::renderable* sphere1 = nullptr;
kl::gl::renderable* metalcube1 = nullptr;
kl::gl::renderable* metalcube2 = nullptr;
kl::gl::renderable* kerv = nullptr;

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
	kl::gl::mesh* cube_mes = renderer.newMesh("res/objects/cube.obj");
	kl::gl::mesh* table_mes = renderer.newMesh("res/objects/table.obj");
	kl::gl::mesh* katana_mes = renderer.newMesh("res/objects/katana.obj");
	kl::gl::mesh* horse_mes = renderer.newMesh("res/objects/horse.obj");
	kl::gl::mesh* sphere_mes = renderer.newMesh("res/objects/sphere.obj");
	kl::gl::mesh* tv_mes = renderer.newMesh("res/objects/tv.obj");

	// Texture creation
	kl::gl::texture* solid1_tex = renderer.newTexture(kl::image(kl::ivec2(1, 1), kl::color(235, 180, 120)));
	kl::gl::texture* solid2_tex = renderer.newTexture(kl::image(kl::ivec2(1, 1), kl::color(120, 180, 200)));
	kl::gl::texture* table_tex = renderer.newTexture("res/textures/table.jpg");
	kl::gl::texture* katana_tex = renderer.newTexture("res/textures/katana.jpg");
	kl::gl::texture* horse_tex = renderer.newTexture("res/textures/horse.jpg");
	kl::gl::texture* tv_tex = renderer.newTexture("res/textures/tv.jpg");
	kl::gl::texture* peace_tex = renderer.newTexture("res/textures/peace.jpg");
	kl::gl::texture* fp_tex = renderer.newTexture("res/textures/firepower.jpg");

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
	metalcube1->physics.angular = kl::vec3(kl::random::getInt(-32, 32), kl::random::getInt(-32, 32), kl::random::getInt(-32, 32));
	metalcube1->physics.gravity = 0;

	metalcube2->geometry.size = kl::vec3(0.5, 0.5, 0.5);
	metalcube2->geometry.rotation = kl::vec3(45, 45, 0);
	metalcube2->geometry.position = kl::vec3(0, -4, -2);
	metalcube2->physics.enabled = true;
	metalcube2->physics.angular = kl::vec3(kl::random::getInt(-32, 32), kl::random::getInt(-32, 32), kl::random::getInt(-32, 32));
	metalcube2->physics.gravity = 0;

	kerv->geometry.size = kl::vec3(5, 5, 5);
	kerv->geometry.rotation = kl::vec3(0, 180, 0);
	kerv->geometry.position = kl::vec3(0, 3, -6);

	// Sun setup
	renderer.sun.direction = kl::vec3(-0.575, -0.75, -2);
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
		kl::gl::setWireframe(true);
	}
	if (keys->f) {
		kl::gl::setWireframe(false);
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