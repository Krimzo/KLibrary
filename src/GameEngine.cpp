#include "KrimzLib.h"


int main() {
	kl::engine testEngine(kl::constant::colors::gray);

	testEngine.EngineStart = [&]() {
		kl::texture tableTexture = testEngine.NewTexture("res/textures/table.png");
		kl::gameobject* table = testEngine.NewObject("table", "res/objects/table.obj", tableTexture);

		table->position = { 0, -0.5, -2 };
		table->physics = true;
		table->angular.y = -36;
		table->gravity = 0;
	};

	testEngine.StartNew(kl::size(900, 900), "Test Engine");


	return 0;
}