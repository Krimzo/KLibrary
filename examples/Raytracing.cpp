#include "KrimzLib.h"


static kl::window window;
static kl::ref<kl::gpu> gpu;
static kl::timer timer;

static kl::dx::mesh screenMesh = nullptr;
static kl::shaders shaders;
static kl::camera camera;

static constexpr int SPHERE_COUNT = 8;
struct Sphere
{
	kl::float3 center;
	float radius = 0.0f;
	kl::float4 color;

	Sphere()
	{
	}
	Sphere(const kl::float3& center, float radius, const kl::float4& color)
		: center(center), radius(radius), color(color)
	{
	}
};
struct PS_CB
{
	kl::float4 frameSize = {};
	kl::mat4 inverseCamera = {};
	kl::float4 cameraPosition = {};
	kl::float4 sunDirection = {};
	Sphere spheres[SPHERE_COUNT] = {};
};
static PS_CB psData = {};

void Start()
{
	window.maximize();

	window.keys.r.press = [&]()
	{
		for (auto& sphere : psData.spheres)
		{
			sphere.color = kl::random::COLOR();
		}
	};

	gpu = kl::make<kl::gpu>(window);

	gpu->bind(gpu->newDepthState(false, false, false));

	gpu->bind(gpu->newRasterState(false, false));

	shaders = gpu->newShaders(kl::file::readString("examples/shaders/raytracing.hlsl"));

	screenMesh = gpu->newVertexBuffer({
		kl::vertex(kl::float3(-1.0f, -1.0f, 0.5f)), kl::vertex(kl::float3(-1.0f, 1.0f, 0.5f)), kl::vertex(kl::float3(1.0f, 1.0f, 0.5f)),
		kl::vertex(kl::float3(-1.0f, -1.0f, 0.5f)), kl::vertex(kl::float3(1.0f, -1.0f, 0.5f)), kl::vertex(kl::float3(1.0f, 1.0f, 0.5f))
		});

	psData.spheres[0] = Sphere({ 0.0f, 0.0f, 20.0f }, 4.00f, kl::random::COLOR());
	psData.spheres[1] = Sphere({ 20.0f, 0.0f, 20.0f }, 1.50f, kl::random::COLOR());
	psData.spheres[2] = Sphere({ -5.0f, 0.0f, 15.0f }, 2.00f, kl::random::COLOR());
	psData.spheres[3] = Sphere({ -5.0f, 0.0f, 25.0f }, 2.50f, kl::random::COLOR());
	psData.spheres[4] = Sphere({ 5.5f, 0.0f, 15.0f }, 3.00f, kl::random::COLOR());
	psData.spheres[5] = Sphere({ 0.0f, 0.0f, 30.0f }, 0.25f, kl::random::COLOR());

	camera.position.y = 5.0f;
	psData.sunDirection = kl::float4(kl::float3(-1.0f, -1.0f, 0.0f).normalize(), 0.0f);

	timer.reset();
}

void Input()
{
	if (window.keys.w)
	{
		camera.moveForward(timer.interval());
	}
	if (window.keys.s)
	{
		camera.moveBack(timer.interval());
	}
	if (window.keys.d)
	{
		camera.moveRight(timer.interval());
	}
	if (window.keys.a)
	{
		camera.moveLeft(timer.interval());
	}
	if (window.keys.e)
	{
		camera.moveUp(timer.interval());
	}
	if (window.keys.q)
	{
		camera.moveDown(timer.interval());
	}

	static bool cameraRotating = false;
	if (window.mouse.lmb || window.mouse.rmb)
	{
		const kl::uint2 frameCenter = window.center();

		if (!cameraRotating)
		{
			window.mouse.position = frameCenter;
		}

		window.mouse.hide();
		cameraRotating = true;

		camera.rotate(window.mouse.position, frameCenter);
		window.mouse.move(frameCenter);
	}
	else if (cameraRotating)
	{
		window.mouse.show();
		cameraRotating = false;
	}
}

void Phys()
{
	for (int i = 0; i < SPHERE_COUNT; i++)
	{
		float oscilation = (std::sin(timer.elapsed() + i) + 1.0f) * 0.5f;
		psData.spheres[i].center.y = (oscilation * (i + 1.0f)) + psData.spheres[i].radius;
	}
}

void Update()
{
	timer.newInterval();

	Phys();

	Input();

	gpu->bind(shaders);

	psData.frameSize = { window.size(), 0.0f, 0.0f };
	psData.inverseCamera = camera.matrix().inverse();
	psData.cameraPosition = { camera.position, 1.0f };

	gpu->autoPixelCBuffer(psData);

	gpu->draw(screenMesh);

	gpu->swap(true);

	window.title(kl::format(int(1.0f / timer.interval())));
}

void Resize(const kl::int2& newSize)
{
	if (gpu && newSize.x > 0 && newSize.y > 0)
	{
		gpu->regenInternal(newSize);
		gpu->viewport(newSize);
		camera.resize(newSize);
	}
}

int main()
{
	window.start = Start;
	window.update = Update;
	window.resize = Resize;
	window.run({ 1600, 900 }, "Raytracing", true, true);
}
