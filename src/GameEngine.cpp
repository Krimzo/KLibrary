#include "KrimzLib.hpp"


int main() {
	kl::engine testEngine(kl::constant::colors::gray);

	kl::gameobject* table1 = nullptr;
	kl::gameobject* table2 = nullptr;
	kl::gameobject* table3 = nullptr;

	testEngine.engineStart = [&]() {
		kl::texture tableTexture = testEngine.newTexture("res/textures/table.png");

		table1 = testEngine.newObject(tableTexture, "res/objects/table.obj");
		table1->position = { -1, -0.5, -2.5 };
		table1->physics = true;
		table1->angular.y = 36;
		table1->gravity = 0;

		table2 = testEngine.newObject(tableTexture, "res/objects/table.obj");
		table2->position = { 0, -0.5, -2.5 };
		table2->physics = true;
		table2->angular.x = -36;
		table2->gravity = 0;

		table3 = testEngine.newObject(tableTexture, "res/objects/table.obj");
		table3->position = { 1, -0.5, -2.5 };
		table3->physics = true;
		table3->angular.y = -36;
		table3->gravity = 0;
	};

	testEngine.engineUpdate = [&]() {
		float tableNewSize = cos(kl::convert::toRadians(table2->rotation.x));
		table2->size = kl::vec3(tableNewSize, tableNewSize, tableNewSize);
	};

	float cameraMoveSpeed = 1;
	float cameraRotateSpeed = 30;
	testEngine.engineInput = [&](kl::key key, kl::mouse mouse) {
		switch (key)
		{
		case 'W':
			testEngine.engineCamera.position.z -= cameraMoveSpeed * testEngine.deltaTime;
			break;
		case 'S':
			testEngine.engineCamera.position.z += cameraMoveSpeed * testEngine.deltaTime;
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
			testEngine.engineCamera.rotation.y -= cameraRotateSpeed * testEngine.deltaTime;
			break;
		case 'J':
			testEngine.engineCamera.rotation.y += cameraRotateSpeed * testEngine.deltaTime;
			break;
		case 'O':
			testEngine.engineCamera.rotation.z -= cameraRotateSpeed * testEngine.deltaTime;
			break;
		case 'U':
			testEngine.engineCamera.rotation.z += cameraRotateSpeed * testEngine.deltaTime;
			break;
		case 'P':
			testEngine.engineCamera.rotation = {};
			break;
		}
	};

	testEngine.startNew(kl::size(1600, 900), "Test Engine");


	return 0;
}