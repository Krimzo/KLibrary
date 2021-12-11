#include "KrimzLib.hpp"


// Frame/window size setup
const kl::size frameSize(1600, 900);

// Function declarations
void Setup();
void Render();

// Main func
int main(int argc, char** argv) {
	// Glut init
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	// Glut window size/pos setup
	glutInitWindowSize(frameSize.width, frameSize.height);
	glutInitWindowPosition(
		kl::constant::ints::ScreenWidth / 2 - frameSize.width / 2,
		kl::constant::ints::ScreenHeight / 2 - frameSize.height / 2 - 50
	);

	// Glut window creation
	int window = glutCreateWindow("Modern OpenGL");

	// Glew init
	kl::id initRes = glewInit();
	if (initRes != GLEW_OK) {
		printf("Glew error: %s\n", glewGetErrorString(initRes));
		return -1;
	}

	// Setup function call
	Setup();

	// Passing the render function
	glutDisplayFunc(Render);

	// Main loop start
	glutMainLoop();

	// Program exit
	return 0;
}

kl::vbo* basicVBO;
kl::shaders* basicShaders;

void Setup() {
	// Enabling face culling
	kl::opengl::setFaceCulling(true);

	// Creating a vertex array
	kl::vec3 vertices[3];
	vertices[0] = kl::vec3(-1, -1, 0);
	vertices[1] = kl::vec3( 0,  1, 0);
	vertices[2] = kl::vec3( 1, -1, 0);

	// Generating the gpu array and passing data
	basicVBO = new kl::vbo();
	basicVBO->setData(vertices, sizeof(vertices), GL_STATIC_DRAW);
	basicVBO->setParsing(0, GL_FLOAT, 3, sizeof(kl::vec3), 0);

	// Creating a shader program
	basicShaders = new kl::shaders("res/shaders/basic.vs", "res/shaders/basic.fs");
}

const kl::color background(50, 50, 50);
void Render() {
	// Clearing the buffers
	kl::opengl::clearBuffers(background);

	// Binding the shaders
	basicShaders->bind();

	// Rendering
	basicVBO->drawArrays(GL_TRIANGLES, 3, 0);

	// Setting redisplay flag for the next frame
	glutPostRedisplay();

	// Swapping the frame buffers
	glutSwapBuffers();
}
