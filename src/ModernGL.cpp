#include "KrimzLib.hpp"


int main() {
	/* Frame setup */
	const kl::size frameSize(1600, 900);
	const kl::colorf background(kl::constant::colors::gray);

	/* Window instance */
	kl::window gameWindow;

	/* Window start definition */
	int wvpUNI = 0;
	kl::camera gameCamera;
	kl::gameobject basicCube;
	gameWindow.start = [&]() {
		// Getting the gl version
		int glMajor, glMinor;
		glGetIntegerv(GL_MAJOR_VERSION, &glMajor);
		glGetIntegerv(GL_MINOR_VERSION, &glMinor);
		printf("Using OpenGL %d.%d\n", glMajor, glMinor);

		// Enabling face culling
		kl::opengl::setFaceCulling(false);

		// Enabling depth test
		kl::opengl::setDepthTest(true);

		// Setting up the camera
		gameCamera.setAspect(frameSize);
		gameCamera.setPlanes(0.01f, 100);
		gameCamera.sensitivity = 0.025f;

		// Setting the object data
		std::vector<kl::vertex> vertexData = {
			kl::vertex(kl::vec3( 0.5,  0.5,  0.5), kl::vec2(1, 1), kl::random::getColor()),
			kl::vertex(kl::vec3(-0.5,  0.5,  0.5), kl::vec2(0, 1), kl::random::getColor()),
			kl::vertex(kl::vec3( 0.5, -0.5,  0.5), kl::vec2(1, 0), kl::random::getColor()),
			kl::vertex(kl::vec3(-0.5, -0.5,  0.5), kl::vec2(0, 0), kl::random::getColor()),
			kl::vertex(kl::vec3( 0.5,  0.5, -0.5), kl::vec2(1, 1), kl::random::getColor()),
			kl::vertex(kl::vec3(-0.5,  0.5, -0.5), kl::vec2(0, 1), kl::random::getColor()),
			kl::vertex(kl::vec3( 0.5, -0.5, -0.5), kl::vec2(1, 0), kl::random::getColor()),
			kl::vertex(kl::vec3(-0.5, -0.5, -0.5), kl::vec2(0, 0), kl::random::getColor())
		};
		std::vector<kl::index> indexData = {
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
		basicCube.setData(vertexData, indexData);

		// Creating the shaders
		basicCube.setShaders(new kl::shaders(
			kl::file::readText("res/shaders/basic.vs"),
			kl::file::readText("res/shaders/basic.fs")
		));
		basicCube.getShaders()->setUniform(basicCube.getShaders()->getUniform("texture0"), 0);
		wvpUNI = basicCube.getShaders()->getUniform("wvp");

		// Loading the dogo texture
		basicCube.setTexture(new kl::texture("res/textures/dogo.png"));
	};

	/* Window update definition */
	kl::time timer;
	bool camMoving = false;
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
			camMoving = true;
			gameWindow.mouse.hide();

			// Fixing the camera jump on the first click
			gameWindow.mouse.position = gameWindow.getCenter();
		}
		if (gameWindow.mouse.rmb) {
			camMoving = false;
			gameWindow.mouse.show();
		}
		if (camMoving) {
			const kl::point frameCenter = gameWindow.getCenter();
			gameCamera.rotate(gameWindow.mouse.position, frameCenter);
			gameWindow.mouse.move(frameCenter);
		}

		/* Setting the cube properties */
		basicCube.size = kl::vec3(1, 1, 1);
		basicCube.position = kl::vec3(0, 0, 2);
		basicCube.rotation = kl::vec3(0, 36 * elapsed, 0);

		/* Setting the matrix uniform */
		basicCube.getShaders()->setUniform(wvpUNI, gameCamera.matrix() * basicCube.matrix());

		/* Rendering */
		basicCube.render();

		/* Updating the fps display */
		gameWindow.setTitle(std::to_string(int(1 / deltaTime)));

		/* Swapping the frame buffers */
		gameWindow.swapFrameBuffers();
	};

	/* Window creation */
	timer.getElapsed();
	timer.stopwatchReset();
	gameWindow.startNew(frameSize, "Game Window", false, true, true);

	return 0;
}
