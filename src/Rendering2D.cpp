#include "KrimzLib.hpp"


// Rendering engine
kl::renderer2D renderer;

// Game objects
kl::object2D* particle1 = nullptr;

// Renderer setup
void setup() {
	// Mesh creation
	kl::mesh* circle_mes = renderer.newMesh("res/objects/circle.obj");

	// Texture creation
	kl::texture* orange_tex = renderer.newTexture(kl::image(kl::ivec2(1, 1), kl::colors::orange));

	// Object creation
	particle1 = renderer.newObject(circle_mes, orange_tex);

	// Object properties setup
	particle1->size = kl::vec2(0.1);
	particle1->position = kl::vec2(0);
	particle1->rotation = 0;
	particle1->velocity.x = kl::random::FLOAT(-1, 1);
	particle1->acceler.y = -1;
}

// Renderer update
void update() {
	// Collision detection
	const float horzLimit = 2 * renderer.getAspect() - particle1->size.x;
	const float vertLimit = 2 - particle1->size.y;
	if ((particle1->position.x - particle1->size.x) <= -horzLimit && particle1->velocity.x < 0) {
		particle1->velocity.x *= -1;
	}
	if ((particle1->position.x + particle1->size.x) >= horzLimit && particle1->velocity.x > 0) {
		particle1->velocity.x *= -1;
	}
	if ((particle1->position.y - particle1->size.y) <= -vertLimit && particle1->velocity.y < 0) {
		particle1->velocity.y *= -1;
	}
	if ((particle1->position.y + particle1->size.y) >= vertLimit && particle1->velocity.y > 0) {
		particle1->velocity.y *= -1;
	}
}

int main() {
	// User start
	renderer.setup = setup;

	// User update
	renderer.update = update;

	// Renderer creation
	renderer.createNew(kl::ivec2(900, 900));


	return 0;
}