#include "KrimzLib/KrimzLib.hpp"


int main() {
	kl::window win;
	kl::gpu* gpu = nullptr;
	kl::shaders* sh = nullptr;
	kl::mesh* tr = nullptr;
	float aspect = 0;

	win.start = [&]() {
		aspect = win.getAspect();

		gpu = new kl::gpu(win.getWND());

		sh = gpu->newShaders("res/shaders/interpolate.hlsl", 0, sizeof(kl::vec4));
		sh->bind();

		const float sideLen = 1;
		const float h = sqrt(0.75f * sideLen * sideLen);
		tr = gpu->newMesh({
			kl::vertex(kl::vec3(sideLen * -0.5f, -h, 0.5f), kl::colors::red),
			kl::vertex(kl::vec3(              0,  h, 0.5f), kl::colors::green),
			kl::vertex(kl::vec3( sideLen * 0.5f, -h, 0.5f), kl::colors::blue)
		});
	};

	win.update = [&]() {
		gpu->clear(kl::colors::gray);

		kl::vec4 mouse(win.mouse.position, 0, 0);
		sh->setPixlData(&mouse);

		tr->draw();

		gpu->swap(true);
	};
	
	win.end = [&]() {
		delete gpu;
	};

	win.startNew(kl::ivec2(1600, 900), "xD", false, true);
}
