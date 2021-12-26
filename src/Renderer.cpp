#include "KrimzLib.hpp"


// Rendering engine
kl::renderer renderer;

// Game objects
kl::renderable* wall = nullptr;
kl::renderable * table = nullptr;
kl::renderable * katanaL = nullptr;
kl::renderable * katanaR = nullptr;
kl::renderable * horse = nullptr;
kl::renderable * sphere1 = nullptr;
kl::array<kl::renderable*> tvs(101);

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
	kl::texture* solid1_tex = renderer.newTexture(kl::image(kl::size(1, 1), kl::color(235, 180, 120)));
	kl::texture* solid2_tex = renderer.newTexture(kl::image(kl::size(1, 1), kl::color(120, 180, 200)));
	kl::texture* table_tex = renderer.newTexture("res/textures/table.jpg");
	kl::texture* katana_tex = renderer.newTexture("res/textures/katana.jpg");
	kl::texture* horse_tex = renderer.newTexture("res/textures/horse.jpg");
	kl::texture* tv_tex = renderer.newTexture("res/textures/tv.jpg");

	// Object creation
	wall = renderer.newObject(cube_mes, solid1_tex);
	table = renderer.newObject(table_mes, table_tex);
	katanaL = renderer.newObject(katana_mes, katana_tex);
	katanaR = renderer.newObject(katana_mes, katana_tex);
	horse = renderer.newObject(horse_mes, horse_tex);
	sphere1 = renderer.newObject(sphere_mes, solid2_tex);

	// Object properties setup
	wall->visible = false;
	wall->geometry.size = kl::vec3(50, 10, 0.05);
	wall->geometry.position = kl::vec3(0, 0, -6);

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

	sphere1->geometry.size = kl::vec3(1, 1, 1);
	sphere1->geometry.rotation = kl::vec3(0, 0, 0);
	sphere1->geometry.position = kl::vec3(0, 2, -2);
	sphere1->physics.enabled = true;
	sphere1->physics.velocity.x = 1;
	sphere1->physics.gravity = 0;

	int tvCounter = 0;
	float tvsStartPos = -tvs.size() / 2.0;
	tvs.forEach([&](auto tv) {
		*tv = renderer.newObject(tv_mes, tv_tex);

		(*tv)->visible = false;
		(*tv)->geometry.size = kl::vec3(2, 2, 2);
		(*tv)->geometry.rotation = kl::vec3(0, 180, 0);
		(*tv)->geometry.position = kl::vec3(tvsStartPos + tvCounter++, 0, -5);
	});
}

// Renderer input
bool camMoving = false;
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
		camMoving = true;
		mouse->hide();

		// Fixing the camera jump on the first click
		mouse->position = renderer.frameCenter();
	}
	if (mouse->rmb) {
		camMoving = false;
		mouse->show();
	}
	if (camMoving) {
		kl::point frameCenter = renderer.frameCenter();
		renderer.cam.rotate(mouse->position, frameCenter);
		mouse->move(frameCenter);
	}

	// Other
	if (keys->v) {
		kl::opengl::setWireframe(true);
	}
	if (keys->f) {
		kl::opengl::setWireframe(false);
	}
	if (keys->comma) {
		wall->visible = false;

		tvs.forEach([&](auto tv) {
			(*tv)->visible = false;
		});
	}
	if (keys->period) {
		wall->visible = true;

		tvs.forEach([&](auto tv) {
			(*tv)->visible = true;
		});
	}
}

// Renderer update
void update() {
	const float horseGravity = 0.1;
	sphere1->physics.velocity += (horse->geometry.position - sphere1->geometry.position) * horseGravity * renderer.deltaT;

	tvs.forEach([&](auto tv) {
		const float x = -(*tv)->geometry.position.x;

		const float y = pow(x * 0.1, 3); //1.5 * (sin((0.5 * x) - (3 * renderer.elapsedT)) + 1);
		(*tv)->geometry.position.y = y;
	});
}

int main() {
	// User start
	renderer.setup = setup;
	
	// User input
	renderer.input = input;

	// User update
	renderer.update = update;

	// Renderer creation
	renderer.createNew(kl::size(1600, 900));


	return 0;
}