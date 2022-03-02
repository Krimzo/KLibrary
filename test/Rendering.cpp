#include "KrimzLib/KrimzLib.hpp"


// Rendering engine
kl::renderer renderer;

// Game objects
kl::entity* wall = nullptr;
kl::entity* table = nullptr;
kl::entity* katanaL = nullptr;
kl::entity* katanaR = nullptr;
kl::entity* horse = nullptr;
kl::entity* sphere = nullptr;
kl::entity* metalcube1 = nullptr;
kl::entity* metalcube2 = nullptr;
kl::entity* kerv = nullptr;

// Skyboxes
kl::skybox* clouds = nullptr;
kl::skybox* night = nullptr;

// Frame size
const kl::int2 frameSize(1600, 900);

// Renderer setup
void setup() {
	// Going into fullscreen
	renderer.setFullscreen(false);

	// Enabling the vSync
	renderer.vSync = true;

	// Skybox creation
	clouds = renderer.newSkybox(
		"",
		"res/textures/skyboxes/clouds/front.jpg",
		"res/textures/skyboxes/clouds/back.jpg",
		"res/textures/skyboxes/clouds/left.jpg",
		"res/textures/skyboxes/clouds/right.jpg",
		"res/textures/skyboxes/clouds/top.jpg",
		"res/textures/skyboxes/clouds/bottom.jpg"
	);
	night = renderer.newSkybox(
		"",
		"res/textures/skyboxes/night/night.jpg"
	);
	renderer.skybox = clouds;

	// Mesh creation
	kl::mesh* cube_mes = renderer.newMesh("res/objects/cube.obj");
	kl::mesh* table_mes = renderer.newMesh("res/objects/table.obj");
	kl::mesh* katana_mes = renderer.newMesh("res/objects/katana.obj");
	kl::mesh* horse_mes = renderer.newMesh("res/objects/horse.obj");
	kl::mesh* sphere_mes = renderer.newMesh("res/objects/sphere.obj");
	kl::mesh* tv_mes = renderer.newMesh("res/objects/tv.obj");

	// Texture creation
	kl::texture* solid1_tex = renderer.newTexture(kl::image(kl::int2(1, 1), kl::color(235, 180, 120)));
	kl::texture* solid2_tex = renderer.newTexture(kl::image(kl::int2(1, 1), kl::color(120, 180, 200)));
	kl::texture* table_tex = renderer.newTexture("res/textures/table.jpg");
	kl::texture* katana_tex = renderer.newTexture("res/textures/katana.jpg");
	kl::texture* horse_tex = renderer.newTexture("res/textures/horse.jpg");
	kl::texture* tv_tex = renderer.newTexture("res/textures/tv.jpg");
	kl::texture* peace_tex = renderer.newTexture("res/textures/peace.jpg");
	kl::texture* fp_tex = renderer.newTexture("res/textures/firepower.jpg");

	// Object creation
	wall = renderer.newEntity("", cube_mes, solid1_tex);
	table = renderer.newEntity("", table_mes, table_tex);
	katanaL = renderer.newEntity("", katana_mes, katana_tex);
	katanaR = renderer.newEntity("", katana_mes, katana_tex);
	horse = renderer.newEntity("", horse_mes, horse_tex);
	sphere = renderer.newEntity("", sphere_mes, solid2_tex);
	metalcube1 = renderer.newEntity("", cube_mes, peace_tex);
	metalcube2 = renderer.newEntity("", cube_mes, fp_tex);
	kerv = renderer.newEntity("", tv_mes, tv_tex);

	// Object properties setup
	wall->size = kl::float3(50, 10, 0.05f);
	wall->position = kl::float3(0, 0, -7);

	table->size = kl::float3(1, 1, 1);
	table->rotation = kl::float3(0, 45, 0);
	table->position = kl::float3(0, -0.5, 2);

	katanaL->size = kl::float3(2, 2, 2);
	katanaL->rotation = kl::float3(0, 180, -42);
	katanaL->position = kl::float3(-1, 0, 2);

	katanaR->size = kl::float3(2, 2, 2);
	katanaR->rotation = kl::float3(0, 0, 42);
	katanaR->position = kl::float3(1, 0, 2);

	horse->size = kl::float3(4, 4, 4);
	horse->position = kl::float3(0, -0.02f, 2);
	horse->physics = true;
	horse->angular.y = 18;

	sphere->size = kl::float3(1, 1, 1);
	sphere->position = kl::float3(0, 2, -2);
	sphere->physics = true;
	sphere->velocity.x = 1;
	sphere->angular.y = -36;

	metalcube1->size = kl::float3(0.5, 0.5, 0.5);
	metalcube1->rotation = kl::float3(45, 45, 0);
	metalcube1->position = kl::float3(0, 4, -2);
	metalcube1->physics = true;
	metalcube1->angular = kl::float3(kl::random::FLOAT(-32, 32), kl::random::FLOAT(-32, 32), kl::random::FLOAT(-32, 32));

	metalcube2->size = kl::float3(0.5, 0.5, 0.5);
	metalcube2->rotation = kl::float3(45, 45, 0);
	metalcube2->position = kl::float3(0, -4, -2);
	metalcube2->physics = true;
	metalcube2->angular = kl::float3(kl::random::FLOAT(-32, 32), kl::random::FLOAT(-32, 32), kl::random::FLOAT(-32, 32));

	kerv->size = kl::float3(5, 5, 5);
	kerv->rotation = kl::float3(0, 180, 0);
	kerv->position = kl::float3(0, 3, -6);

	// Sun setup
	renderer.sun.direction = kl::float3(-0.575f, -0.75f, -2);
}

// Renderer input
bool movingCam = false;
void input(kl::keys* keys, kl::mouse* mouse) {
	// Exit
	if (keys->esc) renderer.stop();

	// Fullscreen
	if (keys->insert) renderer.setFullscreen(true);
	if (keys->delet) renderer.setFullscreen(false);

	// Skyboxes
	if (keys->num0) renderer.skybox = nullptr;
	if (keys->num1) renderer.skybox = clouds;
	if (keys->num2) renderer.skybox = night;

	// Sun
	if (keys->comma) renderer.sun.direction.x += 0.75f * renderer.deltaT;
	if (keys->period) renderer.sun.direction.x -= 0.75f * renderer.deltaT;

	// Movement
	if (keys->w) renderer.camera.moveForward(renderer.deltaT);
	if (keys->s) renderer.camera.moveBack(renderer.deltaT);
	if (keys->d) renderer.camera.moveRight(renderer.deltaT);
	if (keys->a) renderer.camera.moveLeft(renderer.deltaT);
	if (keys->space) renderer.camera.moveUp(renderer.deltaT);
	if (keys->c) renderer.camera.moveDown(renderer.deltaT);

	// Speed
	if (keys->shift) {
		renderer.camera.speed = 5;
	}
	else {
		renderer.camera.speed = 2;
	}

	// Camera rotation
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
		const kl::int2 frameCenter = renderer.frameCenter();
		renderer.camera.rotate(mouse->position, frameCenter);
		mouse->move(frameCenter);
	}
}

// Renderer update
void update() {
	const float horseGrav = 0.1f;
	sphere->velocity += (horse->position - sphere->position) * horseGrav * renderer.deltaT;

	const float r = 2;
	const float sinT = sin(renderer.elapsedT);
	const float cosT = cos(renderer.elapsedT);
	metalcube1->position.x = sphere->position.x + r * sinT;
	metalcube1->position.y = sphere->position.y + r * sinT;
	metalcube1->position.z = sphere->position.z + r * cosT;
	metalcube2->position.x = sphere->position.x - r * sinT;
	metalcube2->position.y = sphere->position.y - r * sinT;
	metalcube2->position.z = sphere->position.z - r * cosT;
}

int main() {
	// User start
	renderer.setup = setup;
	
	// User input
	renderer.input = input;

	// User update
	renderer.update = update;

	// Renderer creation
	renderer.startNew(frameSize);
}
