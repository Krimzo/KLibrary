#include "KrimzLib.h"


static constexpr int SPHERE_COUNT = 10;

struct Sphere {
	kl::Float3 center;
	float radius = 0.0f;
	kl::Float4 color;
};

struct PS_CB {
	kl::Float4 frameSize;
	kl::Mat4 inverseCamera;
	kl::Float4 cameraPosition;
	kl::Float4 sunDirection;
	Sphere spheres[SPHERE_COUNT] = {};
};

int main() {
	kl::Window window = { { 1600, 900 }, "Raytracing" };
	kl::Timer timer;

	kl::GPU gpu = { window.getWindow() };
	kl::Camera camera;

	kl::dx::Buffer screenMesh;
	kl::Shaders shaders;

	PS_CB& psData = *new PS_CB; // Stack warnings, ignore this :)

	window.onResize = [&](kl::UInt2 newSize) {
		if (newSize.x > 0 && newSize.y > 0) {
			gpu.resizeInternal(newSize);
			gpu.setViewport(newSize);
			camera.updateAspectRatio(newSize);
		}
	};

	window.keyboard.r.onPress = [&]() {
		if (window.keyboard.shift) {
			for (auto& sphere : psData.spheres) {
				sphere.color = kl::Random::COLOR();
			}
		}
		else if (window.keyboard.ctrl) {
			for (auto& sphere : psData.spheres) {
				sphere.color = kl::Color(sphere.color).asGray();
			}
		}
		else {
			for (auto& sphere : psData.spheres) {
				sphere = {
					kl::Random::VECTOR3<float>(40.0f) - kl::Float3(20.0f, 20.0f, 20.0f),
					kl::Random::FLOAT(2.75f) + 0.25f,
					kl::Random::COLOR()
				};
			}
		}
	};

	window.mouse.right.onDown = [&]() {
		kl::Ray ray = { camera, window.mouse.getNormalizedPosition() };
		psData.sunDirection = { ray.direction.negate(), 0.0f };
	};

	// Start
	window.maximize();

	shaders = gpu.newShaders(kl::Files::ReadString("Examples/Shaders/Raytracing.hlsl"));

	screenMesh = gpu.generateScreenMesh();

	camera.position.y = 5.0f;
	psData.sunDirection = { kl::Float3(-1.0f, -1.0f, 0.0f).normalize(), 0.0f };

	window.keyboard.r.onPress();

	// Update
	while (window.process(false)) {
		timer.updateInterval();

		{ /* Phys */
			for (int i = 0; i < SPHERE_COUNT; i++) {
				float oscilation = (std::sin(timer.getElapsed() + i) + 1.0f) * 0.5f;
				psData.spheres[i].center.y = (oscilation * (i + 1.0f)) + psData.spheres[i].radius;
			}
		}

		{ /* Input */
			static bool cameraRotating = false;
			if (window.mouse.left) {
				const kl::UInt2 frameCenter = window.getCenter();

				if (cameraRotating) {
					camera.rotate(window.mouse.getPosition(), frameCenter);
				}
				window.mouse.setPosition(frameCenter);

				window.mouse.setHidden(true);
				cameraRotating = true;
			}
			else if (cameraRotating) {
				window.mouse.setHidden(false);
				cameraRotating = false;
			}

			if (window.keyboard.w) {
				camera.moveForward(timer.getInterval());
			}
			if (window.keyboard.s) {
				camera.moveBack(timer.getInterval());
			}
			if (window.keyboard.d) {
				camera.moveRight(timer.getInterval());
			}
			if (window.keyboard.a) {
				camera.moveLeft(timer.getInterval());
			}
			if (window.keyboard.e) {
				camera.moveUp(timer.getInterval());
			}
			if (window.keyboard.q) {
				camera.moveDown(timer.getInterval());
			}
		}

		gpu.clearInternal();

		gpu.bindShaders(shaders);

		psData.frameSize = { window.getSize(), 0.0f, 0.0f };
		psData.inverseCamera = camera.matrix().inverse();
		psData.cameraPosition = { camera.position, 0.0f };

		gpu.autoPixelCBuffer(psData);

		gpu.drawVertexBuffer(screenMesh);

		gpu.swapBuffers(true);

		window.setTitle(kl::Format(int(1.0f / timer.getInterval())));
	}
}
