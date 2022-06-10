#include "KrimzLib.h"


int main() {
	// Window creation
	kl::window window;
	kl::image frame(kl::int2(900));

	// Window size
	const int fpsLimit = 165;

	// Triangle points
	kl::float2 A(50.0f, (frame.height() - 1.0f) / 3.0f);
	kl::color colA = kl::colors::red;
	kl::float2 B((frame.width() - 1.0f) / 3.0f, frame.height() - 51.0f);
	kl::color colB = kl::colors::white;
	kl::float2 C(frame.width() - 51.0f, (frame.height() - 1.0f) * 0.667f);
	kl::color colC = kl::colors::blue;
	kl::float2 D((frame.width() - 1.0f) * 0.667f, 50.0f);
	kl::color colD = kl::colors::green;

	// Triangle creation
	kl::triangle T1(kl::vertex(kl::float3(A, 0.5f)), kl::vertex(kl::float3(B, 0.5f)), kl::vertex(kl::float3(C, 0.5f)));
	const kl::float4 t1Consts = T1.constants();
	kl::triangle T2(kl::vertex(kl::float3(A, 0.5f)), kl::vertex(kl::float3(D, 0.5f)), kl::vertex(kl::float3(C, 0.5f)));
	const kl::float4 t2Consts = T2.constants();

	// Window update setups
	kl::timer timer;
	const float timeToSleep = 1.0f / fpsLimit;
	window.update = [&]() {
		static int frameInd = 0;
		for (int x = frameInd - frame.height(), y = 0; y < frame.height(); x++, y++) {
			// Pixel buffer
			kl::color pixel;

			// Checkig if the point inside a triangle and coloring the pixel
			if (T1.in(t1Consts, kl::float2(x, y))) {
				pixel.r = byte(T1.interpolate(t1Consts, kl::float3(colA.r, colB.r, colC.r), kl::float2(x, y)));
				pixel.g = byte(T1.interpolate(t1Consts, kl::float3(colA.g, colB.g, colC.g), kl::float2(x, y)));
				pixel.b = byte(T1.interpolate(t1Consts, kl::float3(colA.b, colB.b, colC.b), kl::float2(x, y)));
			}
			else if (T2.in(t2Consts, kl::float2(x, y))) {
				pixel.r = byte(T2.interpolate(t2Consts, kl::float3(colA.r, colD.r, colC.r), kl::float2(x, y)));
				pixel.g = byte(T2.interpolate(t2Consts, kl::float3(colA.g, colD.g, colC.g), kl::float2(x, y)));
				pixel.b = byte(T2.interpolate(t2Consts, kl::float3(colA.b, colD.b, colC.b), kl::float2(x, y)));
			}
			else {
				pixel = kl::colors::gray;
			}

			// Drawing the pixel to the frame
			frame.pixel(kl::int2(x, y), pixel);

			// Drawing the rand scanline
			frame.pixel(kl::int2(x + 1, y), kl::random::COLOR());

			// Drawing the yellow scanlinepa
			int yellowStrength = kl::random::INT(0, 256);
			frame.pixel(kl::int2(x + 2, y), kl::color(yellowStrength, yellowStrength, 0));
		}

		// Rendering the frame
		window.draw(frame);

		// Updating the title
		window.title(std::to_string(int((100.0f * frameInd) / (frame.width() + frame.height() - 1))) + "%");

		// Checking the i
		if (++frameInd == frame.width() + frame.height()) {
			window.title("Finished!");
			window.update = []() {};
		}

		// Delta time calculation
		while (timer.elapsed() < timeToSleep);
		timer.reset();
	};

	window.run(frame.size(), "Triangle Interpolation", false, true);
}
