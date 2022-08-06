#include "KrimzLib.h"


struct Stick {
	kl::uint value = 0;
	kl::Color color = {};
};

static kl::Vector<Stick> GenerateSticks(kl::uint count, kl::uint minValueIncl, kl::uint maxValueExcl) {
	kl::uint storedMinValue = maxValueExcl;
	kl::uint storedMaxValue = minValueIncl;
	kl::Vector<Stick> sticks(count);
	for (auto& stick : sticks) {
		stick.value = kl::Random::INT(minValueIncl, maxValueExcl);
		storedMinValue = min(storedMinValue, stick.value);
		storedMaxValue = max(storedMaxValue, stick.value);
	}
	for (auto& stick : sticks) {
		const byte grayValue = byte(kl::Math::Normalize(stick.value, storedMinValue, storedMaxValue) * 255);
		stick.color = { grayValue, grayValue, grayValue };
	}
	return sticks;
}

static void DrawSticks(kl::Image& frame, const kl::Vector<Stick>& sticks) {
	for (int i = 0; i < sticks.size(); i++) {
		frame.drawLine({ i, frame.getHeight() - 1 }, { i, frame.getHeight() - 1 - sticks[i].value }, sticks[i].color);
	}
}

int main() {
	kl::Window window = { { 1600, 900 }, "Visual Sort" };
	kl::Image frame = { window.getSize(), kl::Colors::Gray };

	kl::Vector<Stick> sticks = GenerateSticks(frame.getWidth(), 1, frame.getHeight());

	kl::Thread([&]() {
		window.setTitle("Sorting...");
		for (int i = 0; i < sticks.size() - 1; i++) {
			for (int j = i + 1; j < sticks.size(); j++) {
				if (sticks[j].value < sticks[i].value) {
					std::swap(sticks[i], sticks[j]);
				}
				kl::Time::Wait(0.000005f);
			}
		}
		window.setTitle("Finished!");
	}).detach();

	while (window.process(false)) {
		frame.fill(kl::Colors::Gray);

		DrawSticks(frame, sticks);

		window.drawImage(frame);
	}
}
