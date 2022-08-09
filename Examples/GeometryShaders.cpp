#include "KrimzLib.h"

using namespace kl;


struct VS_CB {
	Mat4 WMatrix;
	Mat4 VPMatrix;
	Float4 miscData;
};

struct PS_CB {
	Float4 objectColor;
	Float4 sunDirection;
};

static Ref<Window> window;
static Ref<GPU> gpu;
static Timer timer;
static Camera camera;

static Float3 sunDirection = { 1.0f, -1.0f, 0.0f };

static Vector<Ref<Entity>> entities;

int main() {
	window = Make<Window>(UInt2(1600, 900), "Geometry Test");
	gpu = Make<GPU>(*window);

	window->onResize = [&](UInt2 newSize) {
		if (newSize.x > 0 && newSize.y > 0) {
			gpu->resizeInternal(newSize);
			gpu->setViewport(newSize);
			camera.updateAspectRatio(newSize);
		}
	};
	window->maximize();

	window->keyboard.v.onPress = [&]() {
		static bool wireframeBound = true;
		static dx::RasterState solidRaster = gpu->newRasterState(false, false);
		static dx::RasterState wireframeRaster = gpu->newRasterState(true, false);
		if (wireframeBound) {
			gpu->bindRasterState(solidRaster);
		}
		else {
			gpu->bindRasterState(wireframeRaster);
		}
		wireframeBound = !wireframeBound;
	};
	window->keyboard.v.onPress();

	String shadersSource = Files::ReadString("Examples/Shaders/GeometryTest.hlsl");
	Shaders defaultShaders = gpu->newShaders(shadersSource);
	dx::GeometryShader geometryShader = gpu->newGeometryShader(shadersSource);

	gpu->bindShaders(defaultShaders);
	gpu->bindGeometryShader(geometryShader);

	dx::Buffer cubeMesh = gpu->newVertexBuffer("Examples/Meshes/Cube.obj");
	dx::Buffer sphereMesh = gpu->newVertexBuffer("Examples/Meshes/Sphere.obj");
	dx::Buffer monkeMesh = gpu->newVertexBuffer("Examples/Meshes/Monke.obj");

	Ref<Entity> mainEntity = Make<Entity>();
	mainEntity->angular.y = -36.0f;
	mainEntity->mesh = monkeMesh;
	mainEntity->color = Colors::Orange;
	entities.push_back(mainEntity);

	window->keyboard.num1.onPress = [&]() {
		mainEntity->mesh = cubeMesh;
	};
	window->keyboard.num2.onPress = [&]() {
		mainEntity->mesh = sphereMesh;
	};
	window->keyboard.num3.onPress = [&]() {
		mainEntity->mesh = monkeMesh;
	};

	float destroyGoal = 0.0f;
	float destroyValue = 0.0f;
	window->keyboard.space.onPress = [&]() {
		destroyGoal = 1.5f;
	};

	camera.position = { -2.0f, 2.0f, -2.0f };
	camera.setForward(camera.position.negate());

	while (window->process(false)) {
		timer.updateInterval();

		for (auto& entity : entities) {
			entity->updatePhysics(timer.getInterval());
		}

		if (destroyValue < destroyGoal) {
			destroyValue += timer.getInterval() * 5.0f;
		}
		else {
			destroyGoal = -INFINITY;
			destroyValue -= timer.getInterval() * 0.25f;
		}

		gpu->clearInternal();

		VS_CB vscb = {};
		vscb.VPMatrix = camera.matrix();

		PS_CB pscb = {};
		pscb.sunDirection = { sunDirection.normalize(), 0.0f };

		for (auto& entity : entities) {
			if (entity) {
				vscb.WMatrix = entity->matrix();
				vscb.miscData.x = max(destroyValue, 0.0f);

				gpu->autoVertexCBuffer(vscb);

				pscb.objectColor = entity->color;
				gpu->autoPixelCBuffer(pscb);

				if (entity->mesh) {
					gpu->drawVertexBuffer(entity->mesh);
				}
			}
		}

		gpu->swapBuffers(true);
	}
}
