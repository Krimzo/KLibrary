#include "KrimzLib.h"


static constexpr float fpsLimit = 165.0f;

int main() {
	kl::Window window = { { 900, 900 }, "Interpolation" };
	kl::Image frame = { window.getSize() };
	kl::Timer timer;

	kl::Float2 A = { 50.0f, (frame.getHeight() - 1.0f) / 3.0f };
	kl::Float2 B = { (frame.getWidth() - 1.0f) / 3.0f, frame.getHeight() - 51.0f };
	kl::Float2 C = { frame.getWidth() - 51.0f, (frame.getHeight() - 1.0f) * 0.667f };
	kl::Float2 D = { (frame.getWidth() - 1.0f) * 0.667f, 50.0f };

	kl::Color colA = kl::Colors::Red;
	kl::Color colB = kl::Colors::White;
	kl::Color colC = kl::Colors::Blue;
	kl::Color colD = kl::Colors::Green;

	kl::Triangle T1 = { { { A, 0.5f } }, { { B, 0.5f } }, { { C, 0.5f } } };
	kl::Triangle T2 = { { { A, 0.5f } }, { { D, 0.5f } }, { { C, 0.5f } } };

	const kl::Float4 t1Consts = T1.getConstants();
	const kl::Float4 t2Consts = T2.getConstants();

	kl::uint frameIndex = 0;
	while (window.process(false)) {
		timer.reset();

		for (kl::uint x = frameIndex - frame.getHeight(), y = 0; y < frame.getHeight(); x++, y++) {
			const kl::Float3 t1Weights = T1.getWeights(t1Consts, { x, y });
			const kl::Float3 t2Weights = T2.getWeights(t2Consts, { x, y });

			kl::Color pixel;
			if (T1.isInTriangle(t1Weights)) {
				pixel.r = byte(T1.interpolate(t1Weights, { colA.r, colB.r, colC.r }));
				pixel.g = byte(T1.interpolate(t1Weights, { colA.g, colB.g, colC.g }));
				pixel.b = byte(T1.interpolate(t1Weights, { colA.b, colB.b, colC.b }));
			}
			else if (T2.isInTriangle(t2Weights)) {
				pixel.r = byte(T2.interpolate(t2Weights, { colA.r, colD.r, colC.r }));
				pixel.g = byte(T2.interpolate(t2Weights, { colA.g, colD.g, colC.g }));
				pixel.b = byte(T2.interpolate(t2Weights, { colA.b, colD.b, colC.b }));
			}
			else {
				pixel = kl::Colors::Gray;
			}

			frame.setPixel({ x, y }, pixel);
			frame.setPixel({ x + 1, y }, kl::Random::COLOR());
			frame.setPixel({ x + 2, y }, kl::Random::COLOR());
		}

		window.drawImage(frame);

		window.setTitle(kl::Format(int((100.0f * frameIndex) / (frame.getWidth() + frame.getHeight() - 1)), "%"));

		kl::Time::Wait((1.0f / fpsLimit) - timer.getElapsed());

		if (++frameIndex == frame.getWidth() + frame.getHeight()) {
			window.setTitle("Finished!");
			while (window.process());
		}
	}
}
