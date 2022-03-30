#include "KrimzLib.h"


int main() {
	// Time table properties
	const int n = 200;
	const float mIncr = 0.15f;
	float m = 0.0f;

	// Circle properties
	kl::color circleColor = kl::colors::defaul;

	// Point properties
	kl::color pointColor = kl::random::COLOR();

	// Line properties
	kl::color lineColor = kl::random::COLOR();

	// Window
	kl::window win;
	kl::image frame(kl::int2(900), kl::colors::gray);
	win.update = [&]() {
		// Color regen
		static bool rWasDown = false;
		if (win.keys.r) {
			if (!rWasDown) {
				pointColor = kl::random::COLOR();
				lineColor = kl::random::COLOR();
			}
			rWasDown = true;
		}
		else {
			rWasDown = false;
		}

		// Clearing the frame
		frame.fill(kl::colors::gray);

		// Drawing the circle
		const float circleRadius = min(frame.width() * 0.5f, frame.height() * 0.5f) - 25.0f;
		frame.drawCircle(frame.size() / 2, circleRadius, circleColor);

		// Calculating the points
		const float pointAngle = 360.0f / n;
		std::vector<kl::int2> circlePoints(n);
		for (int i = 0; i < circlePoints.size(); i++) {
			circlePoints[i] = kl::int2(
				int(cos(kl::convert::toRadians(pointAngle * i + 180.0f)) * circleRadius) + frame.width() / 2,
				int(sin(kl::convert::toRadians(pointAngle * i + 180.0f)) * circleRadius) + frame.height() / 2
			);
		}

		// Drawing the points
		for (kl::int2& point : circlePoints) {
			frame.drawCircle(point, 3.0f, pointColor, true);
		}

		// Drawing the lines
		for (int i = 0; i < circlePoints.size(); i++) {
			const int secondPoint = abs(int(i * m) % n);
			frame.drawLine(circlePoints[i], circlePoints[secondPoint], lineColor);
		}

		// Displaying
		win.drawImage(frame);

		// Incrementing m
		m += mIncr * kl::time::interval();
		win.setTitle("n: " + std::to_string(n) + " m: " + std::to_string(m));
	};

	win.resize = [&](const kl::int2& size) {
		frame.resize(size);
	};

	// Creating the window
	win.startNew(frame.size(), "Times Table", true, true);
}
