#include "KrimzLib.h"


kl::window window;

kl::ref<kl::gpu> gpu;
kl::dx::mesh screenMesh = nullptr;
kl::shaders shaders;

struct PS_CB
{
	kl::float2 frameSize;
	kl::float2 zoom;
	kl::float2 position;
	kl::float2 startPosition;
};

float zoom = 1.0f;
float zoomSpeed = 1.0f;
kl::float2 position = { -0.5f, 0.0f };

const float minZoom = 0.5f;
const float maxZoom = 10000.0f;
const kl::int2 frameSize = { 1600, 900 };

void start()
{
	gpu = kl::make<kl::gpu>(window);

	gpu->bind(gpu->newRasterState(false, false));

	shaders = gpu->newShaders(kl::file::readString("examples/shaders/mandelbrot.hlsl"));

	screenMesh = gpu->newVertexBuffer({
		kl::vertex(kl::float3(-1.0f, -1.0f, 0.5f)), kl::vertex(kl::float3(-1.0f, 1.0f, 0.5f)), kl::vertex(kl::float3(1.0f, 1.0f, 0.5f)),
		kl::vertex(kl::float3(-1.0f, -1.0f, 0.5f)), kl::vertex(kl::float3(1.0f, -1.0f, 0.5f)), kl::vertex(kl::float3(1.0f, 1.0f, 0.5f))
		});
}

kl::timer timer;
void update()
{
	timer.newInterval();
	timer.newElapsed();

	if (window.keys.esc)
	{
		window.stop();
	}
	if (window.keys.r)
	{
		position = { -0.5, 0.0f };
		zoom = 1.0f;
	}
	if (window.keys.w)
	{
		position.y -= (1.0f / zoom) * timer.interval();
	}
	if (window.keys.s)
	{
		position.y += (1.0f / zoom) * timer.interval();
	}
	if (window.keys.d)
	{
		position.x += (1.0f / zoom) * timer.interval();
	}
	if (window.keys.a)
	{
		position.x -= (1.0f / zoom) * timer.interval();
	}
	if (window.mouse.lmb)
	{
		zoom += zoom * zoomSpeed * timer.interval();

		if (zoom < maxZoom)
		{
			const kl::float2 uv = ((kl::float2(window.mouse.position) / frameSize) * 2.0f
				- kl::float2(1.0f, 1.0f)) * (float(frameSize.x) / frameSize.y);
			position += (uv / zoom) * timer.interval();
		}
		else
		{
			zoom = maxZoom;
		}
	}
	if (window.mouse.rmb)
	{
		zoom -= zoom * zoomSpeed * timer.interval();

		if (zoom > minZoom)
		{
			const kl::float2 uv = ((kl::float2(window.mouse.position) / frameSize) * 2.0f
				- kl::float2(1.0f, 1.0f)) * (float(frameSize.x) / frameSize.y);
			position -= (uv / zoom) * timer.interval();
		}
		else
		{
			zoom = minZoom;
		}
	}

	gpu->clear(kl::colors::black);

	gpu->bind(shaders);

	PS_CB pxData = {};
	pxData.frameSize = frameSize;
	pxData.zoom = kl::float2(zoom, zoom);
	pxData.position = position;
	pxData.startPosition = kl::float2();
	gpu->autoPixelCBuffer(pxData);

	gpu->draw(screenMesh);

	gpu->swap(true);

	window.title(kl::format(
		"Fps: ", int(1.0f / timer.interval()),
		" Zoom: ", int(zoom),
		" Position: ", position
	));
}

int main()
{
	window.start = start;
	window.update = update;
	timer.reset();
	timer.interval();
	window.run(frameSize, "Mandelbrot", false, true);
}
