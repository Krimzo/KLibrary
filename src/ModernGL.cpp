#include "KrimzLib.hpp"


/* Frame setup */
const kl::size frameSize(1600, 900);
const kl::colorf background(kl::constant::colors::gray);

/* Public vars */
kl::vbo* basicVBO;
kl::shaders* basicShaders;
int transformationUNI;
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
	kl::triangle basicTriangle(
		kl::vertex(kl::vec3(-1, -1, 0), kl::vec2(0, 0), kl::color(255, 0, 0)),
		kl::vertex(kl::vec3( 0,  1, 0), kl::vec2(0, 0), kl::color(0, 255, 0)),
		kl::vertex(kl::vec3( 1, -1, 0), kl::vec2(0, 0), kl::color(0, 0, 255))
	);

	// Generating the gpu array and passing data
	basicVBO = new kl::vbo();
	basicVBO->setData(basicTriangle.pointer(), sizeof(kl::triangle), GL_STATIC_DRAW);
	basicVBO->setParsing(0, GL_FLOAT, 3, sizeof(kl::vertex), 0);
	basicVBO->setParsing(1, GL_FLOAT, 2, sizeof(kl::vertex), sizeof(kl::vec3));
	basicVBO->setParsing(2, GL_FLOAT, 3, sizeof(kl::vertex), sizeof(kl::vec3) + sizeof(kl::vec2));

	// Creating and binding the shaders
	basicShaders = new kl::shaders("res/shaders/basic.vs", "res/shaders/basic.fs");
	basicShaders->bind();
	transformationUNI = basicShaders->getUniformID("transformation");

	/* Rendering */
	glutDisplayFunc(Render);
	timer.stopwatchReset();
	glutMainLoop();

	/* Program exit */
	return 0;
}

/* Render function definition */
void Render() {
	/* Clearing the buffers */
	kl::opengl::clearBuffers(background);

	// Time calculations
	const float elapsed = timer.staticStopwatchElapsed();

	// Calculating the transformation matrix
	kl::vec3 size(1, 0.5, 1);
	kl::vec3 rotation(0, 0, 90);
	kl::vec3 position(0.5, 0, 0);

	// Setting the uniform data
	basicShaders->setUniformData(transformationUNI, kl::mat4::translate(position) * kl::mat4::rotate(rotation) * kl::mat4::scale(size));

	/* Rendering */
	basicVBO->drawArrays(GL_TRIANGLES, 3, 0);

	/* Updating the fps display */
	glutSetWindowTitle(std::to_string(int(1 / timer.getElapsed())).c_str());

	/* Setting redisplay flag for the next frame */
	glutPostRedisplay();

	/* Swapping the frame buffers */
	glutSwapBuffers();
}
