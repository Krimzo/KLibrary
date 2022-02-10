#include "KrimzLib.hpp"


// DirectX gpu
kl::gpu* gpu = nullptr;

int main() {
	kl::window window;

	window.start = [&]() {
		// DirectX init
		gpu = new kl::gpu(window.getHWND());


	};

	window.update = [&]() {

	};

	window.end = [&]() {
		delete gpu;
	};

	window.startNew(kl::ivec2(1600, 900), "DirectX Testing", false, true);
}
