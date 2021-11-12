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

	double cameraMoveSpeed = 1;
	double cameraRotateSpeed = 30;
	testEngine.EngineInput = [&](kl::key key, kl::mouse mouse) {
		switch (key)
		{
		case 'W':
			testEngine.engineCamera.position.z += cameraMoveSpeed * testEngine.deltaTime;
			break;
		case 'S':
			testEngine.engineCamera.position.z -= cameraMoveSpeed * testEngine.deltaTime;
			break;
		case 'D':
			testEngine.engineCamera.position.x += cameraMoveSpeed * testEngine.deltaTime;
			break;
		case 'A':
			testEngine.engineCamera.position.x -= cameraMoveSpeed * testEngine.deltaTime;
			break;
		case 'Q':
			testEngine.engineCamera.position.y += cameraMoveSpeed * testEngine.deltaTime;
			break;
		case 'E':
			testEngine.engineCamera.position.y -= cameraMoveSpeed * testEngine.deltaTime;
			break;
		case 'R':
			testEngine.engineCamera.position = {};
			break;

		case 'K':
			testEngine.engineCamera.rotation.x += cameraRotateSpeed * testEngine.deltaTime;
			break;
		case 'I':
			testEngine.engineCamera.rotation.x -= cameraRotateSpeed * testEngine.deltaTime;
			break;
		case 'L':
			testEngine.engineCamera.rotation.y += cameraRotateSpeed * testEngine.deltaTime;
			break;
		case 'J':
			testEngine.engineCamera.rotation.y -= cameraRotateSpeed * testEngine.deltaTime;
			break;
		case 'O':
			testEngine.engineCamera.rotation.z += cameraRotateSpeed * testEngine.deltaTime;
			break;
		case 'U':
			testEngine.engineCamera.rotation.z -= cameraRotateSpeed * testEngine.deltaTime;
			break;
		case 'P':
			testEngine.engineCamera.rotation = {};
			break;
		}

		kl::console::HideCursor();
		kl::console::MoveCursor(kl::point(0, 0));
		kl::console::Println(testEngine.engineCamera.position);
		kl::console::Println(testEngine.engineCamera.rotation);
	};

	testEngine.StartNew(kl::size(1600, 900), "Test Engine");


	return 0;
}