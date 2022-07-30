#include "KrimzLib.h"


struct Stick {
	uint value = 0;
	kl::color color;
};

std::vector<Stick> GenSticks(uint count, uint minValueIncl, uint maxValueExcl) {
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

void DrawSticks(kl::image& frame, const std::vector<Stick>& sticks) {
	for (int i = 0; i < sticks.size(); i++) {
		frame.drawLine({ i, frame.height() - 1 }, { i, frame.height() - 1 - sticks[i].value }, sticks[i].color);
	}
}

int main() {
	kl::window window;
	kl::image frame({ 1600, 900 }, kl::colors::gray);

	std::vector<Stick> sticks = GenSticks(frame.width(), 1, frame.height());

	window.start = [&]() {
		std::thread([&]() {
			window.title("Sorting...");
			for (int i = 0; i < sticks.size() - 1; i++) {
				for (int j = i + 1; j < sticks.size(); j++) {
					if (sticks[j].value < sticks[i].value) {
						std::swap(sticks[i], sticks[j]);
					}
					kl::time::wait(0.000005);
				}
			}
			window.title("Finished!");
		}).detach();
	};

	window.update = [&]() {
		frame.fill(kl::colors::gray);
		DrawSticks(frame, sticks);
		window.draw(frame);
	};

	window.run(frame.size(), "Sort", false, true);
}
