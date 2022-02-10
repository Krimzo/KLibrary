#include "KrimzLib.hpp"


int main() {
	// Window
	kl::window window;
	kl::timer timer;
	const bool vSync = true;

	// DirectX
	kl::gpu* gpu = nullptr;

	// Start callback
	window.start = [&]() {
		// DirectX init
		gpu = new kl::gpu(window.getHWND());
	};

	// Frame callback
	window.update = [&]() {
		// Getting time
		const float elapsedT = timer.elapsed();
		const float deltaT = timer.interval();

		// Clearing the buffers
		gpu->clear(kl::colors::gray);

		// Rendering


		// Swapping the buffers
		gpu->swap(vSync);

		// Fps display
		window.setTitle(std::to_string(int(1 / deltaT)));
	};

	// Exit callback
	window.end = [&]() {
		delete gpu;
	};

	// Window creation
	timer.reset();
	timer.interval();
	window.startNew(kl::ivec2(1600, 900), "DirectX Testing", false, true);
}
