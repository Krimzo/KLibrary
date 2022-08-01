#include "KrimzLib.h"


static constexpr int n = 200;
static constexpr float increment = 0.15f;
static float m = 0.0f;

int main() {
	kl::color circleColor = kl::colors::defaul;
	kl::color pointColor = kl::random::COLOR();
	kl::color lineColor = kl::random::COLOR();

	kl::window window = { { 900, 900 }, "Times Table" };
	kl::image frame(window.size(), kl::colors::gray);

	window.resize = [&](const kl::uint2& size) {
		if (size.x > 0 && size.y > 0) {
			frame.size(size);
		}
	};

	window.keys.r.press = [&]() {
		pointColor = kl::random::COLOR();
		lineColor = kl::random::COLOR();
	};

	while (window.process(false)) {
		m += increment * kl::time::interval();

		frame.fill(kl::colors::gray);

		const float circleRadius = min(frame.width() * 0.5f, frame.height() * 0.5f) - 25.0f;
		frame.drawCircle(frame.size() / 2, circleRadius, circleColor);

		const float pointAngle = 360.0f / n;
		std::vector<kl::int2> circlePoints(n);
		for (int i = 0; i < circlePoints.size(); i++) {
			circlePoints[i] = {
				int(std::cos(kl::to::radians(pointAngle * i + 180.0f)) * circleRadius + frame.width() / 2),
				int(std::sin(kl::to::radians(pointAngle * i + 180.0f)) * circleRadius + frame.height() / 2)
			};
		}

		for (auto& point : circlePoints) {
			frame.drawCircle(point, 3.0f, pointColor, true);
		}

		for (int i = 0; i < circlePoints.size(); i++) {
			frame.drawLine(circlePoints[i], circlePoints[std::abs(int64(i * m) % n)], lineColor);
		}

		window.draw(frame);

		window.title(kl::format("n: ", n, " m: ", m));
	}
}
