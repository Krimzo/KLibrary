#include "KrimzLib.h"

using namespace kl;


struct SKY_PS_CB {
	Float4 frameSize;
	Mat4 inverseCamera;
	Float4 cameraPosition;
	Float4 sunDirection;
};

struct PLANE_VS_CB {
	Mat4 WMatrix;
	Mat4 VPMatrix;
	Float4 timeData;
};

struct PLANE_PS_CB {
	Float4 sunDirection;
};

static Ref<Window> window;
static Ref<GPU> gpu;
static Timer timer;
static Camera camera;

static Shaders skyShaders = {};
static Shaders planeShaders = {};
static dx::GeometryShader planeGeometryShader = nullptr;

static Float3 sunDirection = { 1, -1, 0 };

static void SetupInput() {
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

	window->keyboard.r.onPress = [&]() {
		Console::Clear();

		String shaderSource = Files::ReadString("Examples/Shaders/Plane.hlsl");
		Shaders tempDefaultShaders = gpu->newShaders(shaderSource);
		dx::GeometryShader tempGeometryShader = gpu->newGeometryShader(shaderSource);

		if (tempDefaultShaders && tempGeometryShader) {
			gpu->destroy(planeShaders);
			gpu->destroy(planeGeometryShader);
			planeShaders = tempDefaultShaders;
			planeGeometryShader = tempGeometryShader;
		}
	};
	window->keyboard.r.onPress();

	window->mouse.right.onDown = [&]() {
		Ray ray = { camera, window->mouse.getNormalizedPosition() };
		sunDirection = ray.direction.negate();
	};
}

static void CameraMovement() {
	static bool cameraRotating = false;
	if (window->mouse.left) {
		const kl::UInt2 frameCenter = window->getCenter();

		if (cameraRotating) {
			camera.rotate(window->mouse.getPosition(), frameCenter);
		}
		window->mouse.setPosition(frameCenter);

		window->mouse.setHidden(true);
		cameraRotating = true;
	}
	else if (cameraRotating) {
		window->mouse.setHidden(false);
		cameraRotating = false;
	}

	if (window->keyboard.w) {
		camera.moveForward(timer.getInterval());
	}
	if (window->keyboard.s) {
		camera.moveBack(timer.getInterval());
	}
	if (window->keyboard.d) {
		camera.moveRight(timer.getInterval());
	}
	if (window->keyboard.a) {
		camera.moveLeft(timer.getInterval());
	}
	if (window->keyboard.e) {
		camera.moveUp(timer.getInterval());
	}
	if (window->keyboard.q) {
		camera.moveDown(timer.getInterval());
	}
}

int main() {
	window = Make<Window>(UInt2(1600, 900), "Plane Playground");
	gpu = Make<GPU>(*window);

	SetupInput();

	dx::DepthState defaultDepthState = gpu->newDepthState(true, false, false);
	dx::DepthState disabledDepthState = gpu->newDepthState(false, false, false);

	skyShaders = gpu->newShaders(Files::ReadString("Examples/Shaders/Sky.hlsl"));

	dx::Buffer screenMesh = gpu->generateScreenMesh();
	dx::Buffer planeMesh = gpu->generatePlaneMesh(10.0f, 1000);

	camera.position = { -3.5f, 1.5f, -2.5f };
	camera.setForward(camera.position.negate());

	while (window->process(false)) {
		timer.updateInterval();

		CameraMovement();

		gpu->clearInternal();

		/* SKY */
		gpu->bindDepthState(disabledDepthState);

		gpu->bindShaders(skyShaders);
		gpu->bindGeometryShader(nullptr);

		SKY_PS_CB sky_pscb = {};
		sky_pscb.frameSize = Float4(window->getSize(), 0, 0);
		sky_pscb.cameraPosition = Float4(camera.position, 0);
		sky_pscb.inverseCamera = camera.matrix().inverse();
		sky_pscb.sunDirection = { sunDirection.normalize(), 0 };
		gpu->autoPixelCBuffer(sky_pscb);
		
		gpu->drawVertexBuffer(screenMesh);
		/* SKY-END */

		/* PLANE */
		gpu->bindDepthState(defaultDepthState);

		gpu->bindShaders(planeShaders);
		gpu->bindGeometryShader(planeGeometryShader);

		PLANE_VS_CB plane_vscb = {};
		plane_vscb.WMatrix = Mat4();
		plane_vscb.VPMatrix = camera.matrix();
		plane_vscb.timeData.x = timer.getElapsed();
		plane_vscb.timeData.y = timer.getInterval();
		gpu->autoVertexCBuffer(plane_vscb);

		PLANE_PS_CB plane_pscb = {};
		plane_pscb.sunDirection = { sunDirection.normalize(), 0 };
		gpu->autoPixelCBuffer(plane_pscb);

		gpu->drawVertexBuffer(planeMesh);
		/* PLANE-END */

		gpu->swapBuffers(true);
	}
}
