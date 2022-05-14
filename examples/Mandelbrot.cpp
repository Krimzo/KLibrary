#include "KrimzLib.h"


kl::window win;

kl::gpu* gpu = nullptr;
kl::shaders shaders;
ID3D11Buffer* screenMesh = nullptr;

struct PS_CB {
	kl::float2 frameSize;
	kl::float2 zoom;
	kl::float2 pos;
	kl::float2 startPos;
};

float zoom = 1.0f;
float zoomSpeed = 1.0f;
kl::float2 pos(-0.5f, 0.0f);

const float minZoom = 0.5f;
const float maxZoom = 10000.0f;
const kl::int2 frameSize(1600, 900);

void start() {
	// DirectX init
	gpu = new kl::gpu(win.getWND());

	// Setting the raster
	gpu->bind(gpu->newRasterState(false, false));

	// Compiling shaders
	shaders = gpu->newShaders(kl::file::read("examples/shaders/mandelbrot.hlsl"), kl::file::read("examples/shaders/mandelbrot.hlsl"));

	// Creating the box mesh
	screenMesh = gpu->newVertexBuffer({
		kl::vertex(kl::float3(-1.0f, -1.0f, 0.5f)), kl::vertex(kl::float3(-1.0f,  1.0f, 0.5f)), kl::vertex(kl::float3(1.0f, 1.0f, 0.5f)),
		kl::vertex(kl::float3(-1.0f, -1.0f, 0.5f)), kl::vertex(kl::float3(1.0f, -1.0f, 0.5f)), kl::vertex(kl::float3(1.0f, 1.0f, 0.5f))
		});
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
		pos = kl::float2(-0.5, 0.0f);
		zoom = 1.0f;
	}
	if (win.keys.w) {
		pos.y -= (1.0f / zoom) * deltaT;
	}
	if (win.keys.s) {
		pos.y += (1.0f / zoom) * deltaT;
	}
	if (win.keys.d) {
		pos.x += (1.0f / zoom) * deltaT;
	}
	if (win.keys.a) {
		pos.x -= (1.0f / zoom) * deltaT;
	}
	if (win.mouse.lmb) {
		// Calculating the zoom
		zoom += zoom * zoomSpeed * deltaT;

		// Checking the zoom
		if (zoom < maxZoom) {
			const kl::float2 uv = ((kl::float2(win.mouse.position) / frameSize) * 2.0f - 1.0f) * (float(frameSize.x) / frameSize.y);
			pos += (uv / zoom) * deltaT;
		}
		else {
			zoom = maxZoom;
		}
	}
	if (win.mouse.rmb) {
		// Calculating the zoom
		zoom -= zoom * zoomSpeed * deltaT;

		// Checking the zoom
		if (zoom > minZoom) {
			const kl::float2 uv = ((kl::float2(win.mouse.position) / frameSize) * 2.0f - 1.0f) * (float(frameSize.x) / frameSize.y);
			pos -= (uv / zoom) * deltaT;
		}
		else {
			zoom = minZoom;
		}
	}

	// Clearing
	gpu->clear(kl::colors::black);

	// Shaders
	gpu->bind(shaders);

	// CBuffer data
	PS_CB pxData = {};
	pxData.frameSize = frameSize;
	pxData.zoom.x = zoom;
	pxData.pos = pos;
	pxData.startPos.x = 0.0f;
	gpu->autoPixelCBuffer(pxData);

	// Rendering the box
	gpu->draw(screenMesh);

	// Swapping the frame buffers
	gpu->swap(true);

	// Updating the title
	win.setTitle(
		"Fps: " + std::to_string(int(1.0f / deltaT)) +
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
