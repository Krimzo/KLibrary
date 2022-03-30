#include "KrimzLib.h"


int main() {
	// Window creation
	kl::window window;
	kl::image frame(kl::int2(900));

	// Window size
	const int fpsLimit = 165;

	// Triangle points
	kl::float2 A(50.0f, (frame.height() - 1) / 3.0f);
	kl::color colA(255, 0, 0);
	kl::float2 B((frame.width() - 1) / 3.0f, frame.height() - 51.0f);
	kl::color colB(255, 255, 255);
	kl::float2 C(frame.width() - 51.0f, (frame.height() - 1) * 0.667f);
	kl::color colC(0, 0, 255);
	kl::float2 D((frame.width() - 1) * 0.667f, 50.0f);
	kl::color colD(0, 255, 0);

	// Triangle creation
	kl::triangle T1(kl::vertex(kl::float3(A, 0.5f)), kl::vertex(kl::float3(B, 0.5f)), kl::vertex(kl::float3(C, 0.5f)));
	const kl::float4 t1Consts = T1.getConsts();
	kl::triangle T2(kl::vertex(kl::float3(A, 0.5f)), kl::vertex(kl::float3(D, 0.5f)), kl::vertex(kl::float3(C, 0.5f)));
	const kl::float4 t2Consts = T2.getConsts();

	// Window update setups
	kl::timer timer;
	const float timeToSleep = 1.0f / fpsLimit;
	window.update = [&]() {
		static int frameInd = 0;
		for (int x = frameInd - frame.height(), y = 0; y < frame.height(); x++, y++) {
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
			frame.spixel(kl::int2(x, y), pixel);

			// Drawing the rand scanline
			frame.spixel(kl::int2(x + 1, y), kl::random::COLOR());

			// Drawing the yellow scanlinepa
			int yellowStrength = kl::random::INT(0, 256);
			frame.spixel(kl::int2(x + 2, y), kl::color(yellowStrength, yellowStrength, 0));
		}
		
		// Rendering the frame
		window.drawImage(frame);

		// Updating the title
		window.setTitle(std::to_string(int((100.0f * frameInd) / (frame.width() + frame.height() - 1))) + "%");

		// Checking the i
		if (++frameInd == frame.width() + frame.height()) {
			window.setTitle("Finished!");
			window.update = []() {};
		}

		// Delta time calculation
		while (timer.elapsed() < timeToSleep);
		timer.reset();
	};

	// Window start
	window.startNew(frame.size(), "Triangle Interpolation", false, true);
}
