#include "KrimzLib.hpp"


int main() {
	// Window size
	const kl::size windowSize(900, 900);
	const int fpsLimit = 60;

	// Triangle points
	kl::vertex A(kl::vec3(50, (windowSize.height - 1) / 3, 0),                        kl::color(255,   0,   0));
	kl::vertex B(kl::vec3((windowSize.width - 1) / 3, windowSize.height - 51, 0),	  kl::color(255, 255, 255));
	kl::vertex C(kl::vec3(windowSize.width - 51, (windowSize.height - 1) * 0.667, 0), kl::color(  0,   0, 255));
	kl::vertex D(kl::vec3((windowSize.width - 1) * 0.667, 50, 0),                     kl::color(  0, 255,   0));

	// Triangle creation
	kl::triangle T1(A, B, C);
	kl::vec4 t1Consts = T1.getInterConsts();
	kl::triangle T2(A, D, C);
	kl::vec4 t2Consts = T2.getInterConsts();

	// Window creation
	kl::window window;
	kl::image frame(windowSize);

	// Window update setup
	int counter = 0;
	const float timeToSleep = 1.0f / fpsLimit;
	kl::time::staticStopwatchReset();
	window.update = [&]() {
		// Setting x and y
		int x = counter - windowSize.height;
		int y = 0;

		// Interpolating the line
		for (int i = 0; i < windowSize.height; i++) {
			// Pixel buffer
			kl::color pixel = {};

			// Computing the interpolation weights
			kl::vec3 weights1 = T1.getInterWeights(t1Consts, kl::vec2(x, y));
			kl::vec3 weights2 = T2.getInterWeights(t2Consts, kl::vec2(x, y));

			// Checkig if the point inside a triangle and coloring the pixel
			if (T1.inTriangle(weights1)) {
				pixel = kl::convert::toColor(T1.getColor(weights1));
			}
			else if (T2.inTriangle(weights2)) {
				pixel = kl::convert::toColor(T2.getColor(weights2));
			}
			else {
				pixel = kl::constant::colors::gray;
			}

			// Drawing the pixel to the frame
			frame.setPixel(kl::point(x, y), pixel);

			// Drawing the rand scanline
			frame.setPixel(kl::point(x + 1, y), kl::random::getColor());

			// Drawing the yellow scanlinepa
			int yellowStrength = kl::random::getInt(0, 256);
			frame.setPixel(kl::point(x + 2, y), kl::color(yellowStrength, yellowStrength, 0));

			// Updating x and y
			x++;
			y++;
		}
		
		// Rendering the frame
		window.displayImage(frame);

		// Updating the title
		window.setTitle(std::to_string(int((100.0 * counter) / (windowSize.width + windowSize.height - 1))) + "%");

		// Checking the i
		if (++counter == windowSize.width + windowSize.height) {
			window.setTitle("Finished!");
			window.update = []() {};
		}

		// Delta time calculation
		while (kl::time::staticStopwatchElapsed() < timeToSleep);
		kl::time::staticStopwatchReset();
	};

	// Window start
	window.startNew(windowSize, "Triangle Interpolation", false, true);

	return 0;
}
