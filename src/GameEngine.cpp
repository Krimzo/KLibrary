#include "KrimzLib.h"


int main() {
	kl::engine testEngine(kl::constant::colors::gray);

	testEngine.EngineStart = [&]() {
		kl::texture tableTexture = testEngine.NewTexture("res/textures/table.png");

		kl::gameobject* table1 = testEngine.NewObject("table1", "res/objects/table.obj", tableTexture);
		table1->position = { -1, -0.5, -2.5 };
		table1->physics = true;
		table1->angular.y = 36;
		table1->gravity = 0;

		kl::gameobject* table2 = testEngine.NewObject("table2", "res/objects/table.obj", tableTexture);
		table2->position = { 0, -0.5, -2.5 };
		table2->physics = true;
		table2->angular.x = -36;
		table2->gravity = 0;

		kl::gameobject* table3 = testEngine.NewObject("table3", "res/objects/table.obj", tableTexture);
		table3->position = { 1, -0.5, -2.5 };
		table3->physics = true;
		table3->angular.y = -36;
		table3->gravity = 0;
	};

	testEngine.StartNew(kl::size(1600, 900), "Test Engine");


	return 0;
}