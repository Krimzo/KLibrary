#include "KrimzLib.hpp"


int main() {
	// Window size
	const kl::ivec2 windowSize(900, 900);
	const int fpsLimit = 165;

	// Triangle points
	kl::vec2 A(50.0f, (windowSize.y - 1) / 3.0f);
	kl::color colA(255, 0, 0);
	kl::vec2 B((windowSize.x - 1) / 3.0f, windowSize.y - 51.0f);
	kl::color colB(255, 255, 255);
	kl::vec2 C(windowSize.x - 51.0f, (windowSize.y - 1) * 0.667f);
	kl::color colC(0, 0, 255);
	kl::vec2 D((windowSize.x - 1) * 0.667f, 50.0f);
	kl::color colD(0, 255, 0);

	// Triangle creation
	kl::triangle T1(A, B, C);
	T1.calcConsts();
	kl::triangle T2(A, D, C);
	T2.calcConsts();

	// Window creation
	kl::window window;
	kl::image frame(windowSize);

	// Window update setup
	int counter = 0;
	const float timeToSleep = 1.0f / fpsLimit;
	kl::timer timer;
	window.update = [&]() {
		// Setting x and y
		int x = counter - windowSize.y;
		int y = 0;

		// Interpolating the line
		for (int i = 0; i < windowSize.y; i++) {
			// Pixel buffer
			kl::color pixel;
			
			// Checkig if the point inside a triangle and coloring the pixel
			if (T1.inTriangle(kl::vec2(float(x), float(y)))) {
				pixel.r = byte(T1.interpolate(kl::vec3(float(colA.r), float(colB.r), float(colC.r)), kl::vec2(float(x), float(y))));
				pixel.g = byte(T1.interpolate(kl::vec3(float(colA.g), float(colB.g), float(colC.g)), kl::vec2(float(x), float(y))));
				pixel.b = byte(T1.interpolate(kl::vec3(float(colA.b), float(colB.b), float(colC.b)), kl::vec2(float(x), float(y))));
			}
			else if (T2.inTriangle(kl::vec2(float(x), float(y)))) {
				pixel.r = byte(T2.interpolate(kl::vec3(float(colA.r), float(colD.r), float(colC.r)), kl::vec2(float(x), float(y))));
				pixel.g = byte(T2.interpolate(kl::vec3(float(colA.g), float(colD.g), float(colC.g)), kl::vec2(float(x), float(y))));
				pixel.b = byte(T2.interpolate(kl::vec3(float(colA.b), float(colD.b), float(colC.b)), kl::vec2(float(x), float(y))));
			}
			else {
				pixel = kl::colors::gray;
			}

			// Drawing the pixel to the frame
			frame.setPixel(kl::ivec2(x, y), pixel);

			// Drawing the rand scanline
			frame.setPixel(kl::ivec2(x + 1, y), kl::random::COLOR());

			// Drawing the yellow scanlinepa
			int yellowStrength = kl::random::INT(0, 256);
			frame.setPixel(kl::ivec2(x + 2, y), kl::color(yellowStrength, yellowStrength, 0));

			// Updating x and y
			x++;
			y++;
		}
		
		// Rendering the frame
		window.drawImage(frame);

		// Updating the title
		window.setTitle(std::to_string(int((100.0 * counter) / (windowSize.x + windowSize.y - 1))) + "%");

		// Checking the i
		if (++counter == windowSize.x + windowSize.y) {
			window.setTitle("Finished!");
			window.update = []() {};
		}

		// Delta time calculation
		while (timer.elapsed() < timeToSleep);
		timer.reset();
	};

	// Window start
	window.startNew(windowSize, "Triangle Interpolation", false, true);
}
