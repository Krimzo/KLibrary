#include "KrimzLib.hpp"


kl::window win;

kl::mesh* box;
kl::shaders* shad;
kl::uniform frameSize_uni;
kl::uniform zoom_uni;
kl::uniform pos_uni;
kl::uniform startPos_uni;

kl::timer timer;
float deltaT = 0;
float elapsedT = 0;

float zoomSpeed = 1;
float zoom = 1;
kl::vec2 pos(-0.5, 0);

const float minZoom = 0.5f;
const float maxZoom = 5000;

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
	frameSize_uni = shad->getUniform("frameSize");
	zoom_uni = shad->getUniform("zoom");
	pos_uni = shad->getUniform("pos");
	startPos_uni = shad->getUniform("startPos");
}

void update() {
	/* Time calculations */
	deltaT = timer.interval();
	elapsedT = timer.swElapsed();

	/* Getting the screen size */
	kl::vec2 frameSize = win.getSize();

	/* Checking the input */
	if (win.keys.esc) {
		win.stop();
	}
	if (win.keys.insert) {
		win.setFullscreen(true);
		win.resetViewport();
	}
	if (win.keys.delet) {
		win.setFullscreen(false);
		win.resetViewport();
	}
	if (win.keys.r) {
		pos = kl::vec2(-0.5, 0);
		zoom = 1;
	}
	if (win.keys.w) {
		pos.y += (1 / zoom) * deltaT;
	}
	if (win.keys.s) {
		pos.y -= (1 / zoom) * deltaT;
	}
	if (win.keys.d) {
		pos.x += (1 / zoom) * deltaT;
	}
	if (win.keys.a) {
		pos.x -= (1 / zoom) * deltaT;
	}
	if (win.mouse.lmb) {
		// Calculating the zoom
		zoom = zoom + zoom * zoomSpeed * deltaT;

		// Checking the zoom
		if (zoom < maxZoom) {
			// Calculating the position
			kl::vec2 uv(
				((win.mouse.position.x / frameSize.x) * 2 - 1) * (frameSize.x / frameSize.y),
				(((frameSize.y - win.mouse.position.y) / frameSize.y) * 2 - 1)
			);
			pos += (uv / zoom) * deltaT;
		}
		else {
			zoom = maxZoom;
		}
	}
	if (win.mouse.rmb) {
		// Calculating the zoom
		zoom = zoom - zoom * zoomSpeed * deltaT;

		// Checking the zoom
		if (zoom > minZoom) {
			// Calculating the position
			kl::vec2 uv(
				((win.mouse.position.x / frameSize.x) * 2 - 1) * (frameSize.x / frameSize.y),
				(((frameSize.y - win.mouse.position.y) / frameSize.y) * 2 - 1)
			);
			pos -= (uv / zoom) * deltaT;
		}
		else {
			zoom = minZoom;
		}
	}

	/* Setting the uniforms */
	frameSize_uni.setData(frameSize);
	pos_uni.setData(pos);
	zoom_uni.setData(zoom);
	startPos_uni.setData(0);

	/* Rendering the box */
	box->draw();

	/* Updating the fps display */
	win.setTitle(
		"Fps: " + std::to_string(int(1 / deltaT)) +
		" Zoom: " + std::to_string(zoom) +
		" Position: " + std::to_string(pos.x) + " " + std::to_string(pos.y)
	);

	/* Swapping the frame buffers */
	win.swapBuffers();
}

int main() {

	win.start = start;
	win.update = update;
	timer.reset();
	win.startNew(kl::ivec2(1600, 900), "Mandelbrot", false, true, true);

	return 0;
}
