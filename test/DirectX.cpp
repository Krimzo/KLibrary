#include "KrimzLib.hpp"


struct VS_BUFF {
	float aspect;
	float xPos;
	float yPos;
	float ignore;
};

int main() {
	// Window
	kl::window window;
	kl::timer timer;
	bool vSync = true;

	// DirectX
	kl::gpu* gpu = nullptr;
	kl::shaders* shaders = nullptr;
	kl::cbuffer* cb = nullptr;
	kl::mesh* mes = nullptr;
	kl::texture* dogo = nullptr;
	
	// Constant buffer data
	VS_BUFF cbData = {};

	// Start callback
	window.start = [&]() {
		// DirectX init
		gpu = new kl::gpu(window.getHWND());

		// Compiling shaders
		shaders = gpu->newShaders("res/shaders/test.hlsl");
		shaders->bind();

		// CBuffer creation
		cb = gpu->newCBuffer(sizeof(VS_BUFF));
		cb->bind(0);

		// Mesh creation
		mes = gpu->newMesh({
			kl::vertex(kl::vec3(-0.5f, -0.5f, 0.5f), kl::vec2(0.0f, 0.0f)), kl::vertex(kl::vec3(-0.5f, 0.5f, 0.5f), kl::vec2(0.0f, 1.0f)), kl::vertex(kl::vec3(0.5f, 0.5f, 0.5f), kl::vec2(1.0f, 1.0f)),
			kl::vertex(kl::vec3(-0.5f, -0.5f, 0.5f), kl::vec2(0.0f, 0.0f)), kl::vertex(kl::vec3(0.5f, -0.5f, 0.5f), kl::vec2(1.0f, 0.0f)), kl::vertex(kl::vec3(0.5f, 0.5f, 0.5f), kl::vec2(1.0f, 1.0f))
		});

		// Sampler setup
		kl::sampler* samp = gpu->newSampler(true, true);
		samp->bind(0);
		delete samp;

		// Texture creation
		dogo = gpu->newTexture("res/textures/dogo.jpg");
		dogo->bind(0);
	};

	// Frame callback
	window.update = [&]() {
		// Getting time
		const float elapsedT = timer.elapsed();
		const float deltaT = timer.interval();

		// Clearing the buffers
		gpu->clear(kl::colors::gray);

		// CB data clearing
		cbData = {};

		// CB data updating
		cbData.aspect = window.getAspect();
		cbData.xPos = sin(elapsedT);

		// CB data sending
		cb->setData(&cbData);

		// Rendering
		mes->draw();

		// Swapping the buffers
		gpu->swap(vSync);

		// Fps display
		window.setTitle(std::to_string(int(1 / deltaT)));
	};

	// Exit callback
	window.end = [&]() {
		delete dogo;
		delete mes;
		delete cb;
		delete shaders;
		delete gpu;
	};

	// Window creation
	timer.reset();
	timer.interval();
	window.startNew(kl::ivec2(1600, 900), "DirectX Testing", false, true);
}
