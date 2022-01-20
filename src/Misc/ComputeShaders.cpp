#include "KrimzLib.hpp"


int main() {

	// Getting the console window
	HWND hwnd = GetConsoleWindow();

	// Getting the console device context
	HDC hdc = GetDC(hwnd);

	// Init gl
	kl::gl::init(hwnd);

	// Setting the viewport size
	kl::gl::resetViewport(hwnd);

	// WIP
	while (1) {
		//
		kl::gl::clearBuffers(kl::colors::orange);

		//
		SwapBuffers(hdc);
	}

	return 0;
}
