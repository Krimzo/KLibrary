#include "KrimzLib.h"


struct PS_CB {
	kl::Float2 frameSize;
	kl::Float2 zoom;
	kl::Float2 position;
	kl::Float2 startPosition;
};

static constexpr float zoomSpeed = 1.0f;
static constexpr float minZoom = 0.5f;
static constexpr float maxZoom = 10000.0f;

static float zoom = 1.0f;
static kl::Float2 position = { -0.5f, 0.0f };

int main() {
	kl::Window window = { { 1600, 900 }, "Mandelbrot" };
	kl::Timer timer;

	kl::GPU gpu = kl::GPU(window);
	kl::dx::Buffer screenMesh = nullptr;
	kl::Shaders shaders = {};

	window.onResize = [&](kl::UInt2 newSize) {
		if (newSize.x > 0 && newSize.y > 0) {
			gpu.resizeInternal(newSize);
			gpu.setViewport(newSize);
		}
	};
	window.maximize();

	// Start
	shaders = gpu.newShaders(kl::Files::ReadString("Examples/Shaders/Mandelbrot.hlsl"));

	screenMesh = gpu.generateScreenMesh();

	// Update
	while (window.process(false)) {
		timer.updateInterval();

		const kl::UInt2 frameSize = window.getSize();

		// Input
		if (window.keyboard.esc) {
			window.close();
		}
		if (window.keyboard.r) {
			position = { -0.5, 0.0f };
			zoom = 1.0f;
		}
		if (window.keyboard.w) {
			position.y -= (1.0f / zoom) * timer.getInterval();
		}
		if (window.keyboard.s) {
			position.y += (1.0f / zoom) * timer.getInterval();
		}
		if (window.keyboard.d) {
			position.x += (1.0f / zoom) * timer.getInterval();
		}
		if (window.keyboard.a) {
			position.x -= (1.0f / zoom) * timer.getInterval();
		}
		if (window.mouse.left) {
			zoom += zoom * zoomSpeed * timer.getInterval();

			if (zoom < maxZoom) {
				const kl::Float2 uv = ((kl::Float2(window.mouse.getPosition()) / frameSize) * 2.0f
					- kl::Float2(1.0f, 1.0f)) * (float(frameSize.x) / frameSize.y);
				position += (uv / zoom) * timer.getInterval();
			}
			else {
				zoom = maxZoom;
			}
		}
		if (window.mouse.right) {
			zoom -= zoom * zoomSpeed * timer.getInterval();

			if (zoom > minZoom) {
				const kl::Float2 uv = ((kl::Float2(window.mouse.getPosition()) / frameSize) * 2.0f -
					kl::Float2(1.0f, 1.0f)) * (float(frameSize.x) / frameSize.y);
				position -= (uv / zoom) * timer.getInterval();
			}
			else {
				zoom = minZoom;
			}
		}

		// Render
		gpu.clearInternal();

		gpu.bindShaders(shaders);

		PS_CB psData = {};
		psData.frameSize = frameSize;
		psData.zoom = { zoom, zoom };
		psData.position = position;
		psData.startPosition = {};

		gpu.autoPixelCBuffer(psData);

		gpu.drawVertexBuffer(screenMesh);

		gpu.swapBuffers(true);

		window.setTitle(kl::Format(
			"Fps: ", int(1.0f / timer.getInterval()),
			" Zoom: ", int(zoom),
			" Position: ", position
		));
	}
}
