#include "KrimzLib.hpp"


/* Frame setup */
const kl::size frameSize(1600, 900);
const kl::colorf background(kl::constant::colors::gray);

/* Public vars */
kl::vbo* basicVBO;
kl::ibo* basicIBO;
kl::shaders* basicShaders;
int wvpUNI;
kl::camera basicCamera;
kl::time timer;

/* Render function declaration */
void Render();

/* Main func */
int main(int argc, char** argv) {
	/* Glut init */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	/* Glut window size/pos setup */
	glutInitWindowSize(frameSize.width, frameSize.height);
	glutInitWindowPosition(
		kl::constant::ints::ScreenWidth / 2 - frameSize.width / 2,
		kl::constant::ints::ScreenHeight / 2 - frameSize.height / 2 - 50
	);

	/* Glut window creation */
	int window = glutCreateWindow("Modern OpenGL");

	/* Glew init */
	kl::id initRes = glewInit();
	if (initRes != GLEW_OK) {
		printf("Glew error: %s\n", glewGetErrorString(initRes));
		return -1;
	}

	/* Setup */
	// Enabling face culling
	kl::opengl::setFaceCulling(true);

	// Creating a vertex array
	kl::vertex vertices[] = {
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
	basicVBO->setData(vertices, sizeof(vertices), GL_STATIC_DRAW);
	basicVBO->setParsing(0, GL_FLOAT, 3, sizeof(kl::vertex), 0);
	basicVBO->setParsing(1, GL_FLOAT, 2, sizeof(kl::vertex), sizeof(kl::vec3));
	basicVBO->setParsing(2, GL_FLOAT, 3, sizeof(kl::vertex), sizeof(kl::vec3) + sizeof(kl::vec2));

	// Creating an index array
	kl::index indices[] = {
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
	basicIBO->setData(indices, sizeof(indices), GL_STATIC_DRAW);

	// Creating and binding the shaders
	basicShaders = new kl::shaders("res/shaders/basic.vs", "res/shaders/basic.fs");
	basicShaders->bind();
	wvpUNI = basicShaders->getUniformID("wvp");

	// Setting up the camera
	basicCamera.updateAspect(frameSize);
	basicCamera.updatePlanes(0.01, 100);

	/* Rendering setup */
	glutDisplayFunc(Render);
	timer.stopwatchReset();
	timer.getElapsed();
	glutMainLoop();

	/* Program exit */
	return 0;
}

/* Render function definition */
void Render() {
	/* Clearing the buffers */
	kl::opengl::clearBuffers(background);

	/* Time calculations */
	const float elapsed = timer.stopwatchElapsed();
	const float deltaTime = timer.getElapsed();

	/* Calculating the world matrix */
	kl::vec3 size(1, 1, 1);
	kl::vec3 rotation(0, 36 * elapsed, 0);
	kl::vec3 position(0, 0, 1.5);
	kl::mat4 world = kl::mat4::translate(position) * kl::mat4::rotate(rotation) * kl::mat4::scale(size);

	/* Calculating the view matrix */
	kl::mat4 view = basicCamera.getViewMatrix();

	/* Calculating the projection matrix */
	kl::mat4 projection = basicCamera.getProjMatrix();

	/* Sending the transformation matrix */
	basicShaders->setUniformData(wvpUNI, projection * view * world);

	/* Rendering */
	basicIBO->drawElements(basicVBO, GL_TRIANGLES, 36, 0);

	/* Updating the fps display */
	glutSetWindowTitle(std::to_string(int(1 / deltaTime)).c_str());

	/* Setting redisplay flag for the next frame */
	glutPostRedisplay();

	/* Swapping the frame buffers */
	glutSwapBuffers();
}
