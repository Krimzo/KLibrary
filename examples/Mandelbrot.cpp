#include "KrimzLib.h"


kl::window win;

kl::reference<kl::gpu> gpu;
kl::dx::mesh screenMesh = nullptr;
kl::shaders shaders;

struct PS_CB {
	kl::float2 frameSize;
	kl::float2 zoom;
	kl::float2 position;
	kl::float2 startPosition;
};

float zoom = 1.0f;
float zoomSpeed = 1.0f;
kl::float2 pos = { -0.5f, 0.0f };

const float minZoom = 0.5f;
const float maxZoom = 10000.0f;
const kl::int2 frameSize = { 1600, 900 };

void start() {
	gpu = kl::make<kl::gpu>(win);

	gpu->bind(gpu->newRasterState(false, false));

	shaders = gpu->newShaders(kl::file::readString("examples/shaders/mandelbrot.hlsl"));

	screenMesh = gpu->newVertexBuffer({
		kl::vertex(kl::float3(-1.0f, -1.0f, 0.5f)), kl::vertex(kl::float3(-1.0f,  1.0f, 0.5f)), kl::vertex(kl::float3(1.0f, 1.0f, 0.5f)),
		kl::vertex(kl::float3(-1.0f, -1.0f, 0.5f)), kl::vertex(kl::float3(1.0f, -1.0f, 0.5f)), kl::vertex(kl::float3(1.0f, 1.0f, 0.5f))
		});
}

kl::timer timer;
void update() {
	const float deltaT = float(timer.interval());
	const float elapsedT = float(timer.elapsed());

	if (win.keys.esc) {
		win.stop();
	}
	if (win.keys.r) {
		pos = { -0.5, 0.0f };
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
		zoom += zoom * zoomSpeed * deltaT;

		if (zoom < maxZoom) {
			const kl::float2 uv = ((kl::float2(win.mouse.position) / frameSize) * 2.0f - 1.0f) * (float(frameSize.x) / frameSize.y);
			pos += (uv / zoom) * deltaT;
		}
		else {
			zoom = maxZoom;
		}
	}
	if (win.mouse.rmb) {
		zoom -= zoom * zoomSpeed * deltaT;

		if (zoom > minZoom) {
			const kl::float2 uv = ((kl::float2(win.mouse.position) / frameSize) * 2.0f - 1.0f) * (float(frameSize.x) / frameSize.y);
			pos -= (uv / zoom) * deltaT;
		}
		else {
			zoom = minZoom;
		}
	}

	gpu->clear(kl::colors::black);

	gpu->bind(shaders);

	PS_CB pxData = {};
	pxData.frameSize = frameSize;
	pxData.zoom = zoom;
	pxData.position = pos;
	pxData.startPosition = 0.0f;
	gpu->autoPixelCBuffer(pxData);

	gpu->draw(screenMesh);

	gpu->swap(true);

	win.title(kl::format(
		"Fps: ", int(1.0 / deltaT),
		" Zoom: ", int(zoom),
		" Position: ", pos
	));
}

int main() {
	win.start = start;
	win.update = update;
	timer.reset();
	timer.interval();
	win.run(frameSize, "Mandelbrot", false, true);
}
