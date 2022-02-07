#include "KrimzLib.hpp"

// Rendering engine
kl::renderer2D renderer;

// Game objects
std::vector<kl::object2D*> particles;

// Renderer setup
void setup() {
	// Mesh creation
	kl::mesh* circle_mes = renderer.newMesh("res/objects/circle.obj");

	// Texture creation
	kl::texture* orange_tex = renderer.newTexture("res/textures/dogo.jpg");

	// Object creation
	for (int i = 0; i < 200; i++) {
		// Creating a particle
		particles.push_back(renderer.newObject(circle_mes, orange_tex));

		// Initial particle setup
		particles.back()->size = kl::vec2(kl::random::FLOAT(0.01f, 0.02f));
		particles.back()->position = kl::vec2(kl::random::FLOAT(-2, 2), 0);
		particles.back()->velocity = kl::vec2(kl::random::FLOAT(-0.6f, 0.6f), kl::random::FLOAT(-2, 2));
		particles.back()->acceler.y = -2;
	}
}

// Renderer input
void input(kl::keys* keys, kl::mouse* mouse) {
	if (mouse->lmb) {
		// Getting the normalized coords
		kl::vec2 mpos = mouse->normPos(renderer.frameSize());
		mpos.x *= 2 * renderer.getAspect();
		mpos.y *= 2;

		// Updating the particles
		for (int i = 0; i < particles.size(); i++) {
			particles[i]->velocity += (mpos - particles[i]->position) * 0.01f;
		}
	}
}

// Renderer update
void update() {
	// Collision detection
	const float energyCons = 1;
	for (int i = 0; i < particles.size(); i++) {
		const float horzLimit = 2 * renderer.getAspect() - particles[i]->size.x;
		const float vertLimit = 2 - particles[i]->size.y;
		if ((particles[i]->position.x - particles[i]->size.x) < -horzLimit && particles[i]->velocity.x < 0) {
			particles[i]->position.x = -horzLimit + particles[i]->size.x;
			particles[i]->velocity.x *= -energyCons;
		}
		if ((particles[i]->position.x + particles[i]->size.x) > horzLimit && particles[i]->velocity.x > 0) {
			particles[i]->position.x = horzLimit - particles[i]->size.x;
			particles[i]->velocity.x *= -energyCons;
		}
		if ((particles[i]->position.y - particles[i]->size.y) < -vertLimit && particles[i]->velocity.y < 0) {
			particles[i]->position.y = -vertLimit + particles[i]->size.y;
			particles[i]->velocity.y *= -energyCons;
		}
		if ((particles[i]->position.y + particles[i]->size.y) > vertLimit && particles[i]->velocity.y > 0) {
			particles[i]->position.y = vertLimit - particles[i]->size.y;
			particles[i]->velocity.y *= -energyCons;
		}
	}
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
}
