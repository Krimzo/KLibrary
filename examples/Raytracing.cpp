#include "KrimzLib.h"


kl::window win;

kl::gpu* gpu = nullptr;
ID3D11VertexShader* vertSha = nullptr;
ID3D11PixelShader* pixlSha = nullptr;
ID3D11Buffer* cbuff = nullptr;
ID3D11Buffer* screenMes = nullptr;

kl::timer timer;
float deltaT = 0.0f;
float elapsedT = 0.0f;

kl::camera camera;

kl::sphere spheres[6] = {};

int selected = -1;

struct PS_SP {
	// Geometry
	kl::float3 center;
	float radius = 0;

	// Light
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
	// Window maximize
	win.maximize();

	// Gpu creation
	gpu = new kl::gpu(win.getWND());

	// Disabling depth test
	gpu->bind(gpu->newDepthState(false, false, false));

	// Raster setup
	gpu->bind(gpu->newRasterState(false, false));

	// Compiling shaders
	ID3D11InputLayout* defaultLayout = nullptr;
	vertSha = gpu->newVertexShader(kl::file::read("examples/shaders/raytracing.hlsl"), &defaultLayout);
	pixlSha = gpu->newPixelShader(kl::file::read("examples/shaders/raytracing.hlsl"));
	cbuff = gpu->newConstBuffer(sizeof(PS_CB));
	gpu->bind(defaultLayout);

	// Screen mesh creation
	screenMes = gpu->newVertBuffer({
		kl::vertex(kl::float3(-1.0f, -1.0f, 0.5f)), kl::vertex(kl::float3(-1.0f, 1.0f, 0.5f)), kl::vertex(kl::float3(1.0f, 1.0f, 0.5f)),
		kl::vertex(kl::float3(-1.0f, -1.0f, 0.5f)), kl::vertex(kl::float3(1.0f, -1.0f, 0.5f)), kl::vertex(kl::float3(1.0f, 1.0f, 0.5f))
	});

	// Sphere setup
	spheres[0] = kl::sphere(kl::float3( 0.0f, -9005.0f, 20.0f),  9000.0f, kl::color( 50,  50,  50), 0.0f, 0.0f);
	spheres[1] = kl::sphere(kl::float3( 0.0f,     0.0f, 20.0f),    4.00f, kl::random::COLOR(), 0.8f, 0.0f);
	spheres[2] = kl::sphere(kl::float3(-5.0f,    -1.0f, 15.0f),    2.00f, kl::random::COLOR(), 0.9f, 0.0f);
	spheres[3] = kl::sphere(kl::float3(-5.0f,     0.0f, 25.0f),    3.00f, kl::random::COLOR(), 0.9f, 0.0f);
	spheres[4] = kl::sphere(kl::float3( 5.5f,     0.0f, 15.0f),    3.00f, kl::random::COLOR(), 0.9f, 0.0f);
	spheres[5] = kl::sphere(kl::float3( 0.0f,    20.0f, 30.0f),    0.25f, kl::color(255, 255, 255), 0.0f, 3.0f);
}

void Input() {
	// Selection
	if (win.keys.num0) selected = -1;
	if (win.keys.num1) selected =  0;
	if (win.keys.num2) selected =  1;
	if (win.keys.num3) selected =  2;
	if (win.keys.num4) selected =  3;
	if (win.keys.num5) selected =  4;
	if (win.keys.num6) selected =  5;

	// Speed
	if (win.keys.shift) {
		camera.speed = 5;
	}
	else {
		camera.speed = 2;
	}

	// Movement
	if (selected == -1) {
		if (win.keys.w) camera.moveForward(deltaT);
		if (win.keys.s) camera.moveBack(deltaT);
		if (win.keys.d) camera.moveRight(deltaT);
		if (win.keys.a) camera.moveLeft(deltaT);
		if (win.keys.space) camera.moveUp(deltaT);
		if (win.keys.c) camera.moveDown(deltaT);
	}
	else {
		if (win.keys.w) spheres[selected].center += camera.getForward() * (camera.speed * deltaT);
		if (win.keys.s) spheres[selected].center -= camera.getForward() * (camera.speed * deltaT);
		if (win.keys.d) spheres[selected].center += camera.getRight() * (camera.speed * deltaT);
		if (win.keys.a) spheres[selected].center -= camera.getRight() * (camera.speed * deltaT);
		if (win.keys.space) spheres[selected].center += kl::float3::pos_y * (camera.speed * deltaT);
		if (win.keys.c) spheres[selected].center -= kl::float3::pos_y * (camera.speed * deltaT);
	}

	// Camera rotation
	static bool camMoving = false;
	if (win.mouse.lmb || win.mouse.rmb) {
		// Getting the frame center
		const kl::int2 frameCenter = win.getCenter();

		// Fixing the camera jump on the first click
		if (!camMoving) {
			win.mouse.position = frameCenter;
		}

		// Saving info
		win.mouse.hide();
		camMoving = true;

		// Updating the cam
		camera.rotate(win.mouse.position, frameCenter);
		win.mouse.move(frameCenter);
	}
	else {
		// Saving info
		win.mouse.show();
		camMoving = false;
	}
}

void Phys() {
	for (int i = 1; i < 5; i++) {
		spheres[i].center.y = 1.5f * sin(elapsedT + i);
	}
}

void Update() {
	// Time
	deltaT = timer.interval();
	elapsedT = timer.elapsed();

	// Input
	Input();

	// Physics
	Phys();

	// Binding
	gpu->bind(vertSha);
	gpu->bind(pixlSha);
	gpu->bindPixlCBuff(cbuff, 0);

	// Setting data
	PS_CB psData = {};
	psData.frameSize = kl::float4(win.getSize(), 0.0f, 0.0f);
	psData.invCam = camera.matrix().inverse();
	psData.camPos = camera.position;
	for (int i = 0; i < 6; i++) {
		psData.spheres[i].center = spheres[i].center;
		psData.spheres[i].radius = spheres[i].radius;
		psData.spheres[i].color = spheres[i].color;
		psData.spheres[i].reflectivity = spheres[i].reflectivity;
		psData.spheres[i].emission = spheres[i].calcEmiss();
	}
	gpu->setBuffData(cbuff, &psData);

	// Raytracing
	gpu->draw(screenMes);

	// Buffer swap
	gpu->swap(true);

	// Fps display
	win.setTitle(std::to_string(int(1 / deltaT)));
}

void Resize(const kl::int2& newSize) {
	if (gpu && newSize.x > 0 && newSize.y > 0) {
		gpu->regenBuffers(newSize);
		gpu->viewport(kl::int2(0), newSize);
		camera.aspect = float(newSize.x) / newSize.y;
	}
}

int main() {
	win.start = Start;
	win.update = Update;
	win.resize = Resize;
	timer.elapsed();
	win.startNew(kl::int2(1600, 900), "Raytracing", true, true);
}
