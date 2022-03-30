#include "KrimzLib.h"


kl::window win;

kl::gpu* gpu = nullptr;
ID3D11VertexShader* vertSha = nullptr;
ID3D11PixelShader* pixlSha = nullptr;
ID3D11Buffer* cbuff = nullptr;
ID3D11Buffer* box = nullptr;

struct PS_CB {
	kl::float2 frameSize;
	kl::float2 zoom;
	kl::float2 pos;
	kl::float2 startPos;
};

float zoom = 1;
float zoomSpeed = 1;
kl::float2 pos(-0.5, 0);

const float minZoom = 0.5f;
const float maxZoom = 10000;
const kl::int2 frameSize(1600, 900);

void start() {
	// DirectX init
	gpu = new kl::gpu(win.getWND());

	// Setting the raster
	gpu->bind(gpu->newRasterState(false, false));

	// Compiling shaders
	ID3D11InputLayout* defaultLayout = nullptr;
	vertSha = gpu->newVertexShader(kl::file::read("examples/shaders/mandelbrot.hlsl"), &defaultLayout);
	pixlSha = gpu->newPixelShader(kl::file::read("examples/shaders/mandelbrot.hlsl"));
	cbuff = gpu->newConstBuffer(sizeof(PS_CB));
	gpu->bind(defaultLayout);

	// Creating the box mesh
	std::vector<kl::vertex> boxVertices = {
		kl::vertex(kl::float3(-1.0f, -1.0f, 0.5f)), kl::vertex(kl::float3(-1.0f,  1.0f, 0.5f)), kl::vertex(kl::float3(1.0f, 1.0f, 0.5f)),
		kl::vertex(kl::float3(-1.0f, -1.0f, 0.5f)), kl::vertex(kl::float3( 1.0f, -1.0f, 0.5f)), kl::vertex(kl::float3(1.0f, 1.0f, 0.5f))
	};
	box = gpu->newVertBuffer(boxVertices);
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
		pos = kl::float2(-0.5, 0);
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
			const kl::float2 uv = ((kl::float2(win.mouse.position) / frameSize) * 2.0f - 1.0f) * (float(frameSize.x) / frameSize.y);
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
			const kl::float2 uv = ((kl::float2(win.mouse.position) / frameSize) * 2.0f - 1.0f) * (float(frameSize.x) / frameSize.y);
			pos -= (uv / zoom) * deltaT;
		}
		else {
			zoom = minZoom;
		}
	}

	// Clearing
	gpu->clear(kl::colors::black);

	// Binding
	gpu->bind(vertSha);
	gpu->bind(pixlSha);
	gpu->bindPixlCBuff(cbuff, 0);

	// CBuffer data
	PS_CB pxdata = {};
	pxdata.frameSize = frameSize;
	pxdata.zoom.x = zoom;
	pxdata.pos = pos;
	pxdata.startPos.x = 0;
	gpu->setBuffData(cbuff, &pxdata);

	// Rendering the box
	gpu->draw(box);

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
