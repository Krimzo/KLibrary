#include "KrimzLib.hpp"


kl::window win;

kl::shaders* shad;
kl::mesh* box;

kl::timer timer;
float deltaT = 0;
float elapsedT = 0;

void start() {
	/* Compiling shaders */
	shad = new kl::shaders(
		kl::file::read("res/shaders/mandelbrot.vert"),
		kl::file::read("res/shaders/mandelbrot.frag")
	);
	shad->use();

	/* Creating the box mesh */
	std::vector<kl::vertex> boxVertices = {
		kl::vertex(kl::vec3(-1, -1, 0)), kl::vertex(kl::vec3(-1,  1, 0)), kl::vertex(kl::vec3(1, 1, 0)),
		kl::vertex(kl::vec3(-1, -1, 0)), kl::vertex(kl::vec3( 1, -1, 0)), kl::vertex(kl::vec3(1, 1, 0))
	};
	box = new kl::mesh(boxVertices);

	/* Getting shader uniforms */

}

kl::color background = kl::colors::gray;
void update() {
	/* Time calculations */
	deltaT = timer.interval();
	elapsedT = timer.swElapsed();

	/* Clearing the default buffers */
	kl::gl::clearBuffers(background);

	/* Rendering the box */
	box->draw();

	/* Updating the fps display */
	win.setTitle(std::to_string(int(1 / deltaT)));

	/* Swapping the frame buffers */
	win.swapBuffers();
}

int main() {

	win.start = start;
	win.update = update;
	timer.reset();
	win.startNew(kl::ivec2(1600, 900), "Mandelbrot", false, false, true);

	return 0;
}
