#include "KrimzLib.h"


static constexpr int n = 200;
static constexpr float increment = 0.15f;
static float m = 0.0f;

int main() {
	kl::Color circleColor = kl::Colors::Default;
	kl::Color pointColor = kl::Random::COLOR();
	kl::Color lineColor = kl::Random::COLOR();

	kl::Window window = { { 900, 900 }, "Times Table" };
	kl::Image frame(window.getSize(), kl::Colors::Gray);

	window.onResize = [&](const kl::UInt2& size) {
		if (size.x > 0 && size.y > 0) {
			frame.setSize(size);
		}
	};

	window.keyboard.r.onPress = [&]() {
		pointColor = kl::Random::COLOR();
		lineColor = kl::Random::COLOR();
	};

	while (window.process(false)) {
		m += increment * kl::Time::GetInterval();

		frame.fill(kl::Colors::Gray);

		const float circleRadius = min(frame.getWidth() * 0.5f, frame.getHeight() * 0.5f) - 25.0f;
		frame.drawCircle(frame.getSize() / 2, circleRadius, circleColor);

		const float pointAngle = 360.0f / n;
		kl::Vector<kl::Int2> circlePoints(n);
		for (int i = 0; i < circlePoints.size(); i++) {
			circlePoints[i] = {
				int(std::cos(kl::Math::ToRadians(pointAngle * i + 180.0f)) * circleRadius + frame.getWidth() / 2),
				int(std::sin(kl::Math::ToRadians(pointAngle * i + 180.0f)) * circleRadius + frame.getHeight() / 2)
			};
		}

		for (auto& point : circlePoints) {
			frame.drawCircle(point, 3.0f, pointColor, true);
		}

		for (int i = 0; i < circlePoints.size(); i++) {
			frame.drawLine(circlePoints[i], circlePoints[std::abs(kl::int64(i * m) % n)], lineColor);
		}

		window.drawImage(frame);

		window.setTitle(kl::Format("n: ", n, " m: ", m));
	}
}
