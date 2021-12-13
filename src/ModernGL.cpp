#include "KrimzLib.hpp"


int main(int argc, char** argv) {
	/* Frame setup */
	const kl::size frameSize(1600, 900);
	const kl::colorf background(kl::constant::colors::gray);

	/* Window instance */
	kl::window gameWindow;

	/* Window start definition */
	int wvpUNI = 0;
	kl::vbo* basicVBO = nullptr;
	kl::ibo* basicIBO = nullptr;
	kl::shaders* basicShaders = nullptr;
	kl::camera basicCamera;
	kl::gameobject basicCube;
	gameWindow.start = [&]() {
		// Enabling face culling
		kl::opengl::setFaceCulling(true);

		// Setting up the camera
		basicCamera.setAspect(frameSize);
		basicCamera.setPlanes(0.01, 100);

		// Setting the vertex data
		basicCube.vertices = {
			kl::vertex(kl::vec3( 0.5,  0.5,  0.5), kl::random::getColor()),
			kl::vertex(kl::vec3(-0.5,  0.5,  0.5), kl::random::getColor()),
			kl::vertex(kl::vec3( 0.5, -0.5,  0.5), kl::random::getColor()),
			kl::vertex(kl::vec3(-0.5, -0.5,  0.5), kl::random::getColor()),
			kl::vertex(kl::vec3( 0.5,  0.5, -0.5), kl::random::getColor()),
			kl::vertex(kl::vec3(-0.5,  0.5, -0.5), kl::random::getColor()),
			kl::vertex(kl::vec3( 0.5, -0.5, -0.5), kl::random::getColor()),
			kl::vertex(kl::vec3(-0.5, -0.5, -0.5), kl::random::getColor())
		};
		basicVBO = new kl::vbo();
		basicVBO->setData(basicCube.vertexData(), basicCube.vertexDataSize(), GL_STATIC_DRAW);
		basicVBO->setParsing(0, GL_FLOAT, 3, sizeof(kl::vertex), 0);
		basicVBO->setParsing(1, GL_FLOAT, 2, sizeof(kl::vertex), sizeof(kl::vec3));
		basicVBO->setParsing(2, GL_FLOAT, 3, sizeof(kl::vertex), sizeof(kl::vec3) + sizeof(kl::vec2));

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
		basicIBO = new kl::ibo();
		basicIBO->setData(basicCube.indexData(), basicCube.indexDataSize(), GL_STATIC_DRAW);

		// Creating and binding the shaders
		basicShaders = new kl::shaders("res/shaders/basic.vs", "res/shaders/basic.fs");
		basicShaders->bind();
		wvpUNI = basicShaders->getUniformID("wvp");
	};

	/* Window update definition */
	kl::time timer;
	gameWindow.update = [&]() {
		/* Clearing the buffers */
		kl::opengl::clearBuffers(background);

		/* Time calculations */
		const float elapsed = timer.stopwatchElapsed();
		const float deltaTime = timer.getElapsed();

		/* Input */
		if (gameWindow.keys.w) {
			basicCamera.moveForward(deltaTime);
		}
		if (gameWindow.keys.s) {
			basicCamera.moveBack(deltaTime);
		}
		if (gameWindow.keys.d) {
			basicCamera.moveRight(deltaTime);
		}
		if (gameWindow.keys.a) {
			basicCamera.moveLeft(deltaTime);
		}
		if (gameWindow.keys.space) {
			basicCamera.moveUp(deltaTime);
		}
		if (gameWindow.keys.c) {
			basicCamera.moveDown(deltaTime);
		}

		/* Calculating the world matrix */
		basicCube.size = kl::vec3(1, 1, 1);
		basicCube.position = kl::vec3(0, 0, 1.5);
		basicCube.rotation = kl::vec3(0, 36 * elapsed, 0);
		kl::mat4 world = basicCube.worldMatrix();

		/* Calculating the view matrix */
		kl::mat4 view = basicCamera.viewMatrix();

		/* Calculating the projection matrix */
		kl::mat4 projection = basicCamera.projMatrix();

		/* Sending the transformation matrix */
		basicShaders->setUniformData(wvpUNI, projection * view * world);

		/* Rendering */
		basicIBO->drawElements(basicVBO, GL_TRIANGLES, 36, 0);

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
