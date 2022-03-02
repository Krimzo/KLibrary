#include "KrimzLib/KrimzLib.hpp"


int main() {
	// Window size
	const kl::int2 windowSize(900, 900);
	const int fpsLimit = 165;

	// Triangle points
	kl::float2 A(50.0f, (windowSize.y - 1) / 3.0f);
	kl::color colA(255, 0, 0);
	kl::float2 B((windowSize.x - 1) / 3.0f, windowSize.y - 51.0f);
	kl::color colB(255, 255, 255);
	kl::float2 C(windowSize.x - 51.0f, (windowSize.y - 1) * 0.667f);
	kl::color colC(0, 0, 255);
	kl::float2 D((windowSize.x - 1) * 0.667f, 50.0f);
	kl::color colD(0, 255, 0);

	// Triangle creation
	kl::triangle T1(kl::vertex(kl::float3(A, 0.5f)), kl::vertex(kl::float3(B, 0.5f)), kl::vertex(kl::float3(C, 0.5f)));
	const kl::float4 t1Consts = T1.getConsts();
	kl::triangle T2(kl::vertex(kl::float3(A, 0.5f)), kl::vertex(kl::float3(D, 0.5f)), kl::vertex(kl::float3(C, 0.5f)));
	const kl::float4 t2Consts = T2.getConsts();

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
			if (T1.inTriangle(t1Consts, kl::float2(float(x), float(y)))) {
				pixel.r = byte(T1.interpolate(t1Consts, kl::float3(float(colA.r), float(colB.r), float(colC.r)), kl::float2(float(x), float(y))));
				pixel.g = byte(T1.interpolate(t1Consts, kl::float3(float(colA.g), float(colB.g), float(colC.g)), kl::float2(float(x), float(y))));
				pixel.b = byte(T1.interpolate(t1Consts, kl::float3(float(colA.b), float(colB.b), float(colC.b)), kl::float2(float(x), float(y))));
			}
			else if (T2.inTriangle(t2Consts, kl::float2(float(x), float(y)))) {
				pixel.r = byte(T2.interpolate(t2Consts, kl::float3(float(colA.r), float(colD.r), float(colC.r)), kl::float2(float(x), float(y))));
				pixel.g = byte(T2.interpolate(t2Consts, kl::float3(float(colA.g), float(colD.g), float(colC.g)), kl::float2(float(x), float(y))));
				pixel.b = byte(T2.interpolate(t2Consts, kl::float3(float(colA.b), float(colD.b), float(colC.b)), kl::float2(float(x), float(y))));
			}
			else {
				pixel = kl::colors::gray;
			}

			// Drawing the pixel to the frame
			frame.setPixel(kl::int2(x, y), pixel);

			// Drawing the rand scanline
			frame.setPixel(kl::int2(x + 1, y), kl::random::COLOR());

			// Drawing the yellow scanlinepa
			int yellowStrength = kl::random::INT(0, 256);
			frame.setPixel(kl::int2(x + 2, y), kl::color(yellowStrength, yellowStrength, 0));

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
