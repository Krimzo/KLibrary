#include "KrimzLib.hpp"


int main(int argc, char** argv) {
	/* Frame setup */
	const kl::size frameSize(1600, 900);
	const kl::colorf background(kl::constant::colors::gray);

	/* Window instance */
	kl::window gameWindow;

	/* Window start definition */
	int wvpUNI = 0;
	int tex0UNI = 0;
	kl::vbo* VBO = nullptr;
	kl::ibo* IBO = nullptr;
	kl::shaders* shaders = nullptr;
	kl::texture* dogoTexture = nullptr;
	kl::camera gameCamera;
	kl::gameobject basicCube;
	gameWindow.start = [&]() {
		// Enabling face culling
		kl::opengl::setFaceCulling(false);

		// Enabling depth test
		kl::opengl::setDepthTest(true);

		// Setting up the camera
		gameCamera.setAspect(frameSize);
		gameCamera.setPlanes(0.01f, 100);

		// Setting the vertex data
		basicCube.vertices = {
			kl::vertex(kl::vec3( 0.5,  0.5,  0.5), kl::vec2(1, 1), kl::random::getColor()),
			kl::vertex(kl::vec3(-0.5,  0.5,  0.5), kl::vec2(0, 1), kl::random::getColor()),
			kl::vertex(kl::vec3( 0.5, -0.5,  0.5), kl::vec2(1, 0), kl::random::getColor()),
			kl::vertex(kl::vec3(-0.5, -0.5,  0.5), kl::vec2(0, 0), kl::random::getColor()),
			kl::vertex(kl::vec3( 0.5,  0.5, -0.5), kl::vec2(1, 1), kl::random::getColor()),
			kl::vertex(kl::vec3(-0.5,  0.5, -0.5), kl::vec2(0, 1), kl::random::getColor()),
			kl::vertex(kl::vec3( 0.5, -0.5, -0.5), kl::vec2(1, 0), kl::random::getColor()),
			kl::vertex(kl::vec3(-0.5, -0.5, -0.5), kl::vec2(0, 0), kl::random::getColor())
		};
		VBO = new kl::vbo();
		VBO->setData(basicCube.vertexData(), basicCube.vertexDataSize(), GL_STATIC_DRAW);
		VBO->setParsing(0, GL_FLOAT, 3, sizeof(kl::vertex), 0);
		VBO->setParsing(1, GL_FLOAT, 2, sizeof(kl::vertex), sizeof(kl::vec3));
		VBO->setParsing(2, GL_FLOAT, 3, sizeof(kl::vertex), sizeof(kl::vec3) + sizeof(kl::vec2));

		// Setting the index data
		basicCube.indices = {
			0, 1, 3,
			0, 3, 2,
			0, 4, 5,
			0, 5, 1,
			0, 2, 6,
			0, 6, 4,
			7, 3, 1,
			7, 1, 5,
			7, 4, 6,
			7, 5, 4,
			7, 6, 2,
			7, 2, 3
		};
		IBO = new kl::ibo();
		IBO->setData(basicCube.indexData(), basicCube.indexDataSize(), GL_STATIC_DRAW);

		// Creating and binding the shaders
		shaders = new kl::shaders("res/shaders/basic.vs", "res/shaders/basic.fs");
		shaders->bind();
		wvpUNI = shaders->getUniform("wvp");
		tex0UNI = shaders->getUniform("texture0");

		// Loading and binding the dogo texture
		dogoTexture = new kl::texture();
		dogoTexture->load("res/textures/dogo.png");
		dogoTexture->bind(GL_TEXTURE0);
		shaders->setUniform(tex0UNI, 0);
	};

	/* Window update definition */
	kl::time timer;
	bool movingCam = false;
	gameWindow.update = [&]() {
		/* Clearing the buffers */
		kl::opengl::clearBuffers(background);

		/* Time calculations */
		const float elapsed = timer.stopwatchElapsed();
		const float deltaTime = timer.getElapsed();

		/* Keyboard input */
		if (gameWindow.keys.w) {
			gameCamera.moveForward(deltaTime);
		}
		if (gameWindow.keys.s) {
			gameCamera.moveBack(deltaTime);
		}
		if (gameWindow.keys.d) {
			gameCamera.moveRight(deltaTime);
		}
		if (gameWindow.keys.a) {
			gameCamera.moveLeft(deltaTime);
		}
		if (gameWindow.keys.space) {
			gameCamera.moveUp(deltaTime);
		}
		if (gameWindow.keys.c) {
			gameCamera.moveDown(deltaTime);
		}
		if (gameWindow.keys.shift) {
			gameCamera.speed = 5;
		}
		else {
			gameCamera.speed = 2;
		}

		/* Mouse input */
		if (gameWindow.mouse.lmb) {
			movingCam = true;
			gameWindow.mouse.hide();

			// Fixing the camera jump on the first click
			gameWindow.mouse.position = gameWindow.getCenter();
		}
		if (gameWindow.mouse.rmb) {
			movingCam = false;
			gameWindow.mouse.show();
		}
		if (movingCam) {
			const kl::point frameCenter = gameWindow.getCenter();
			gameCamera.rotate(frameCenter, gameWindow.mouse.position);
			gameWindow.mouse.move(frameCenter);
		}

		/* Calculating the world matrix */
		basicCube.size = kl::vec3(1, 1, 1);
		basicCube.position = kl::vec3(0, 0, 1.5);
		basicCube.rotation = kl::vec3(0, 36 * elapsed, 0);
		kl::mat4 world = basicCube.worldMatrix();

		/* Calculating the view matrix */
		kl::mat4 view = gameCamera.viewMatrix();

		/* Calculating the projection matrix */
		kl::mat4 projection = gameCamera.projMatrix();

		/* Setting the matrix uniform */
		shaders->setUniform(wvpUNI, projection * view * world);

		/* Rendering */
		IBO->drawElements(VBO, GL_TRIANGLES, 36, 0);

		/* Updating the fps display */
		gameWindow.setTitle(std::to_string(int(1 / deltaTime)));

		/* Swapping the frame buffers */
		gameWindow.swapFrameBuffers();
	};

	/* Window creation */
	timer.getElapsed();
	timer.stopwatchReset();
	gameWindow.startNew(frameSize, "Game Window", false, true, true);

	/* Cleanup */
	delete VBO;
	delete IBO;
	delete shaders;
	delete dogoTexture;

	return 0;
}
