#include "KrimzLib.hpp"


kl::window win;

kl::gpu* gpu = nullptr;
kl::shaders* sha = nullptr;
kl::mesh* box = nullptr;

struct PS_CB {
	kl::vec2 frameSize;
	kl::vec2 zoom;
	kl::vec2 pos;
	kl::vec2 startPos;
};

float zoom = 1;
float zoomSpeed = 1;
kl::vec2 pos(-0.5, 0);

const float minZoom = 0.5f;
const float maxZoom = 10000;
const kl::ivec2 frameSize(1600, 900);

void start() {
	// DirectX init
	gpu = new kl::gpu(win.getHWND(), 1);

	// Setting the raster
	kl::raster* ras = gpu->newRaster(false, false, true);
	ras->bind();
	delete ras;

	// Compiling shaders
	sha = gpu->newShaders("res/shaders/mandelbrot.hlsl", 0, sizeof(PS_CB));

	// Creating the box mesh
	std::vector<kl::vertex> boxVertices = {
		kl::vertex(kl::vec3(-1, -1, 0.5)), kl::vertex(kl::vec3(-1,  1, 0.5)), kl::vertex(kl::vec3(1, 1, 0.5)),
		kl::vertex(kl::vec3(-1, -1, 0.5)), kl::vertex(kl::vec3( 1, -1, 0.5)), kl::vertex(kl::vec3(1, 1, 0.5))
	};
	box = gpu->newMesh(boxVertices);
}

kl::timer timer;
void update() {
	// Time calculations
	const float deltaT = timer.interval();
	const float elapsedT = timer.elapsed();

	/* Checking the input */
	if (win.keys.esc) {
		win.stop();
	}
	if (win.keys.r) {
		pos = kl::vec2(-0.5, 0);
		zoom = 1;
	}
	if (win.keys.w) {
		pos.y -= (1 / zoom) * deltaT;
	}
	if (win.keys.s) {
		pos.y += (1 / zoom) * deltaT;
	}
	if (win.keys.d) {
		pos.x += (1 / zoom) * deltaT;
	}
	if (win.keys.a) {
		pos.x -= (1 / zoom) * deltaT;
	}
	if (win.mouse.lmb) {
		// Calculating the zoom
		zoom += (zoom * zoomSpeed * deltaT);

		// Checking the zoom
		if (zoom < maxZoom) {
			const kl::vec2 uv = ((kl::vec2(win.mouse.position) / frameSize) * 2.0f - 1.0f) * (float(frameSize.x) / frameSize.y);
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
			const kl::vec2 uv = ((kl::vec2(win.mouse.position) / frameSize) * 2.0f - 1.0f) * (float(frameSize.x) / frameSize.y);
			pos -= (uv / zoom) * deltaT;
		}
		else {
			zoom = minZoom;
		}
	}

	// Clearing
	gpu->clear(kl::colors::black);

	// CBuffer data
	PS_CB pxdata = {};
	pxdata.frameSize = frameSize;
	pxdata.zoom.x = zoom;
	pxdata.pos = pos;
	pxdata.startPos.x = 0;
	sha->setPixlData(&pxdata);

	// Rendering the box
	box->draw();

	// Swapping the frame buffers
	gpu->swap(true);

	// Updating the title
	win.setTitle(
		"Fps: " + std::to_string(int(1 / deltaT)) +
		" Zoom: " + std::to_string(int(zoom)) +
		" Position: " + std::to_string(pos.x) + " " + std::to_string(pos.y)
	);
}

int main() {
	win.start = start;
	win.update = update;
	timer.reset();
	timer.interval();
	win.startNew(frameSize, "Mandelbrot", false, true);
}
