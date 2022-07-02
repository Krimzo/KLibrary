#include "KrimzLib.h"


constexpr double fpsLimit = 165.0;

int main() {
	kl::window window;
	kl::image frame(900);

	kl::float2 A = { 50.0f, (frame.height() - 1.0f) / 3.0f };
	kl::float2 B = { (frame.width() - 1.0f) / 3.0f, frame.height() - 51.0f };
	kl::float2 C = { frame.width() - 51.0f, (frame.height() - 1.0f) * 0.667f };
	kl::float2 D = { (frame.width() - 1.0f) * 0.667f, 50.0f };

	kl::color colA = kl::colors::red;
	kl::color colB = kl::colors::white;
	kl::color colC = kl::colors::blue;
	kl::color colD = kl::colors::green;

	kl::triangle T1(kl::vertex(kl::float3{ A, 0.5f }), kl::vertex(kl::float3{ B, 0.5f }), kl::vertex(kl::float3{ C, 0.5f }));
	kl::triangle T2(kl::vertex(kl::float3{ A, 0.5f }), kl::vertex(kl::float3{ D, 0.5f }), kl::vertex(kl::float3{ C, 0.5f }));

	const kl::float4 t1Consts = T1.constants();
	const kl::float4 t2Consts = T2.constants();

	kl::timer timer;
	window.update = [&]() {
		timer.reset();

		static uint frameInd = 0;
		for (uint x = frameInd - frame.height(), y = 0; y < frame.height(); x++, y++) {
			const kl::float3 t1Weights = T1.weights(t1Consts, { x, y });
			const kl::float3 t2Weights = T2.weights(t2Consts, { x, y });

			kl::color pixel;
			if (T1.in(t1Weights)) {
				pixel.r = byte(T1.interpolate(t1Weights, { colA.r, colB.r, colC.r }));
				pixel.g = byte(T1.interpolate(t1Weights, { colA.g, colB.g, colC.g }));
				pixel.b = byte(T1.interpolate(t1Weights, { colA.b, colB.b, colC.b }));
			}
			else if (T2.in(t2Weights)) {
				pixel.r = byte(T2.interpolate(t2Weights, { colA.r, colD.r, colC.r }));
				pixel.g = byte(T2.interpolate(t2Weights, { colA.g, colD.g, colC.g }));
				pixel.b = byte(T2.interpolate(t2Weights, { colA.b, colD.b, colC.b }));
			}
			else {
				pixel = kl::colors::gray;
			}
			frame.pixel({ x, y }, pixel);
			frame.pixel({ x + 1, y }, kl::random::COLOR());
			frame.pixel({ x + 2, y }, kl::random::COLOR());
		}

		window.draw(frame);

		window.title(kl::format(int((100.0f * frameInd) / (frame.width() + frame.height() - 1)), "%"));

		if (++frameInd == frame.width() + frame.height()) {
			window.title("Finished!");
			window.update = []() {};
		}

		static const double timeToSleep = 1.0 / fpsLimit;
		while (timer.elapsed() < timeToSleep);
	};

	window.run(frame.size(), "Triangle Interpolation", false, true);
}
