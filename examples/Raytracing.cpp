#include "KrimzLib.h"


kl::window win;
kl::reference<kl::gpu> gpu;
kl::dx::mesh screenMesh = nullptr;
kl::shaders shaders;
kl::camera camera;

kl::timer timer;
float deltaT = 0.0f;
float elapsedT = 0.0f;

kl::sphere spheres[6] = {};

int selected = -1;

struct PS_SP {
	kl::float3 center;
	float radius = 0;
	kl::float3 color;
	float reflectivity = 0;
	kl::float4 emission = 0;
};

struct PS_CB {
	kl::float4 frameSize;
	kl::mat4 invCam;
	kl::float4 camPos;
	PS_SP spheres[6] = {};
};

void Start() {
	win.maximize();

	win.keys.r.press = [&]() {
		for (int i = 1; i < 5; i++) {
			spheres[i].color = kl::random::COLOR();
		}
	};

	gpu = kl::make<kl::gpu>(win);

	gpu->bind(gpu->newDepthState(false, false, false));

	gpu->bind(gpu->newRasterState(false, false));

	shaders = gpu->newShaders(kl::file::readString("examples/shaders/raytracing.hlsl"));

	screenMesh = gpu->newVertexBuffer({
		kl::vertex(kl::float3(-1.0f, -1.0f, 0.5f)), kl::vertex(kl::float3(-1.0f, 1.0f, 0.5f)), kl::vertex(kl::float3(1.0f, 1.0f, 0.5f)),
		kl::vertex(kl::float3(-1.0f, -1.0f, 0.5f)), kl::vertex(kl::float3(1.0f, -1.0f, 0.5f)), kl::vertex(kl::float3(1.0f, 1.0f, 0.5f))
		});

	spheres[0] = kl::sphere({ 0.0f, -9005.0f, 20.0f }, 9000.0f, kl::colors::gray, 0.0f, 0.0f);
	spheres[1] = kl::sphere({ 0.0f, 0.0f, 20.0f }, 4.00f, kl::random::COLOR(), 0.8f, 0.0f);
	spheres[2] = kl::sphere({ -5.0f, -1.0f, 15.0f }, 2.00f, kl::random::COLOR(), 0.9f, 0.0f);
	spheres[3] = kl::sphere({ -5.0f, 0.0f, 25.0f }, 3.00f, kl::random::COLOR(), 0.9f, 0.0f);
	spheres[4] = kl::sphere({ 5.5f, 0.0f, 15.0f }, 3.00f, kl::random::COLOR(), 0.9f, 0.0f);
	spheres[5] = kl::sphere({ 0.0f, 20.0f, 30.0f }, 0.25f, kl::colors::white, 0.0f, 3.0f);
}

void Input() {
	if (win.keys.num0) {
		selected = -1;
	}
	if (win.keys.num1) {
		selected = 0;
	}
	if (win.keys.num2) {
		selected = 1;
	}
	if (win.keys.num3) {
		selected = 2;
	}
	if (win.keys.num4) {
		selected = 3;
	}
	if (win.keys.num5) {
		selected = 4;
	}
	if (win.keys.num6) {
		selected = 5;
	}

	if (win.keys.shift) {
		camera.speed = 5;
	}
	else {
		camera.speed = 2;
	}

	if (selected == -1) {
		if (win.keys.w) {
			camera.moveForward(deltaT);
		}
		if (win.keys.s) {
			camera.moveBack(deltaT);
		}
		if (win.keys.d) {
			camera.moveRight(deltaT);
		}
		if (win.keys.a) {
			camera.moveLeft(deltaT);
		}
		if (win.keys.e) {
			camera.moveUp(deltaT);
		}
		if (win.keys.q) {
			camera.moveDown(deltaT);
		}
	}
	else {
		if (win.keys.w) {
			spheres[selected].center += camera.forward() * (camera.speed * deltaT);
		}
		if (win.keys.s) {
			spheres[selected].center -= camera.forward() * (camera.speed * deltaT);
		}
		if (win.keys.d) {
			spheres[selected].center += camera.right() * (camera.speed * deltaT);
		}
		if (win.keys.a) {
			spheres[selected].center -= camera.right() * (camera.speed * deltaT);
		}
		if (win.keys.e) {
			spheres[selected].center += kl::float3(0.0f, 1.0f, 0.0f) * (camera.speed * deltaT);
		}
		if (win.keys.q) {
			spheres[selected].center -= kl::float3(0.0f, 1.0f, 0.0f) * (camera.speed * deltaT);
		}
	}

	static bool camMoving = false;
	if (win.mouse.lmb || win.mouse.rmb) {
		const kl::uint2 frameCenter = win.center();

		if (!camMoving) {
			win.mouse.position = frameCenter;
		}

		win.mouse.hide();
		camMoving = true;

		camera.rotate(win.mouse.position, frameCenter);
		win.mouse.move(frameCenter);
	}
	else {
		win.mouse.show();
		camMoving = false;
	}
}

void Phys() {
	for (int i = 1; i < 5; i++) {
		spheres[i].center.y = 1.5f * std::sin(elapsedT + i);
	}
}

void Update() {
	deltaT = float(timer.interval());
	elapsedT = float(timer.elapsed());

	Input();

	Phys();

	gpu->bind(shaders);

	PS_CB psData = {};
	psData.frameSize = { win.size(), 0.0f, 0.0f };
	psData.invCam = camera.matrix().inverse();
	psData.camPos = { camera.position, 1.0f };
	for (int i = 0; i < 6; i++) {
		psData.spheres[i].center = spheres[i].center;
		psData.spheres[i].radius = spheres[i].radius;
		psData.spheres[i].color = spheres[i].color;
		psData.spheres[i].reflectivity = spheres[i].reflectivity;
		psData.spheres[i].emission = { spheres[i].emissive(), 1.0f };
	}
	gpu->autoPixelCBuffer(psData);

	gpu->draw(screenMesh);

	gpu->swap(true);

	win.title(kl::format(int(1.0 / deltaT)));
}

void Resize(const kl::int2& newSize) {
	if (gpu && newSize.x > 0 && newSize.y > 0) {
		gpu->regenInternal(newSize);
		gpu->viewport({ 0 }, newSize);
		camera.aspect = float(newSize.x) / newSize.y;
	}
}

int main() {
	win.start = Start;
	win.update = Update;
	win.resize = Resize;
	timer.interval();
	timer.reset();
	win.run({ 1600, 900 }, "Raytracing", true, true);
}
