#include "KrimzLib.h"


struct Stick {
	uint value = 0;
	kl::color color;
};

static std::vector<Stick> GenerateSticks(uint count, uint minValueIncl, uint maxValueExcl) {
	uint storedMinValue = maxValueExcl;
	uint storedMaxValue = minValueIncl;
	std::vector<Stick> sticks(count);
	for (auto& stick : sticks) {
		stick.value = kl::random::INT(minValueIncl, maxValueExcl);
		storedMinValue = min(storedMinValue, stick.value);
		storedMaxValue = max(storedMaxValue, stick.value);
	}
	for (auto& stick : sticks) {
		const byte grayValue = byte(kl::math::normalize(stick.value, storedMinValue, storedMaxValue) * 255);
		stick.color = kl::color(grayValue, grayValue, grayValue);
	}
	return sticks;
}

static void DrawSticks(kl::image& frame, const std::vector<Stick>& sticks) {
	for (int i = 0; i < sticks.size(); i++) {
		frame.drawLine({ i, frame.height() - 1 }, { i, frame.height() - 1 - sticks[i].value }, sticks[i].color);
	}
}

int main() {
	kl::window window = { { 1600, 900 }, "Visual Sort" };
	kl::image frame(window.size(), kl::colors::gray);

	std::vector<Stick> sticks = GenerateSticks(frame.width(), 1, frame.height());

	std::thread([&]() {
		window.title("Sorting...");
		for (int i = 0; i < sticks.size() - 1; i++) {
			for (int j = i + 1; j < sticks.size(); j++) {
				if (sticks[j].value < sticks[i].value) {
					std::swap(sticks[i], sticks[j]);
				}
				kl::time::wait(0.000005f);
			}
		}
		window.title("Finished!");
	}).detach();

	while (window.process(false)) {
		frame.fill(kl::colors::gray);
		DrawSticks(frame, sticks);
		window.draw(frame);
	}
}
