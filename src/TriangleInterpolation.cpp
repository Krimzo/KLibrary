#include "KrimzLib.h"


int main() {
	// Window size
	const kl::size windowSize(900, 900);
	const int fpsLimit = 60;

	// Triangle points
	kl::vertex A(50, (windowSize.height - 1) / 3);
	A.c = kl::color(215, 135, 65);
	kl::vertex B((windowSize.width - 1) / 3, windowSize.height - 51);
	B.c = kl::color(30, 95, 65);
	kl::vertex C(windowSize.width - 51, (windowSize.height - 1) * 0.667);
	C.c = kl::color(185, 115, 140);
	kl::vertex D((windowSize.width - 1) * 0.667, 50);
	D.c = kl::color(55, 100, 120);

	// Triangle creation
	kl::triangle T1(A, B, C);
	T1.computeBaryConst();
	kl::triangle T2(A, D, C);
	T2.computeBaryConst();

	// Window creation
	kl::window window;
	kl::image frame(windowSize);

	// Window update setup
	int counter = 0;
	const double timeToSleep = 1.0 / fpsLimit;
	kl::time::StaticStopwatchReset();
	window.WindowUpdate = [&]() {
		// Setting x and y
		int x = counter - windowSize.height;
		int y = 0;

		// Interpolating the line
		for (int i = 0; i < windowSize.height; i++) {
			// Pixel buffer
			kl::color pixel = {};

			// Computing the interpolation weights
			kl::vec3 weights1 = T1.getBaryWeights(kl::vec2(x, y));
			kl::vec3 weights2 = T2.getBaryWeights(kl::vec2(x, y));

			// Checkig if the point inside a triangle and coloring the pixel
			if (T1.inTriangle(weights1)) {
				pixel = T1.interpolateColor(weights1);
			}
			else if (T2.inTriangle(weights2)) {
				pixel = T2.interpolateColor(weights2);
			}
			else {
				pixel = kl::constant::colors::gray;
			}

			// Drawing the pixel to the frame
			frame.SetPixel(kl::point(x, y), pixel);

			// Drawing the rand scanline
			frame.SetPixel(kl::point(x + 1, y), kl::random::Color());

			// Drawing the yellow scanlinepa
			int yellowStrength = kl::random::Int(0, 256);
			frame.SetPixel(kl::point(x + 2, y), kl::color(yellowStrength, yellowStrength, 0));

			// Updating x and y
			x++;
			y++;
		}
		
		// Rendering the frame
		window.RenderImage(frame);

		// Updating the title
		window.SetTitle(std::to_string(int((100.0 * counter) / (windowSize.width + windowSize.height - 1))) + "%");

		// Checking the i
		if (++counter == windowSize.width + windowSize.height) {
			window.SetTitle("Finished!");
			window.WindowUpdate = []() {};
		}

		// Delta time calculation
		while (kl::time::StaticStopwatchElapsed() < timeToSleep);
		kl::time::StaticStopwatchReset();
	};

	// Window start
	window.StartNew(windowSize, "Triangle Interpolation", false, true);

	return 0;
}
