#include "KrimzLib/KrimzLib.hpp"


int main() {
	// Time table properties
	int n = 200;
	float m = 0;
	float mIncrement = 0.1f;

	// Circle properties
	float radiusBias = 25;
	kl::color circleColor(220, 220, 220);

	// Point properties
	float pointRadius = 3;
	kl::color pointColor(175, 20, 55);

	// Line properties
	kl::color lineColor(20, 175, 120);

	// Window properties
	kl::int2 windowSize(1600, 900);

	// Window
	kl::window window;
	kl::image frame(windowSize, kl::colors::gray);
	window.update = [&]() {
		// Clearing the frame
		frame.fill(kl::colors::gray);

		// Circle position and size calculations
		kl::int2 circleCenter(windowSize.x / 2, windowSize.y / 2);
		float circleRadius = min(windowSize.x * 0.5f, windowSize.y * 0.5f) - radiusBias;

		// Drawing the circle
		frame.drawCircle(circleCenter, circleRadius, circleColor);

		// Calculating the points
		float pointAngle = 360.0f / n;
		std::vector<kl::int2> circlePoints(n);
		int pointCounter = 0;
		for (kl::int2& point : circlePoints) {
			const int pointX = int(cos(kl::convert::toRadians(pointAngle * pointCounter + 180)) * circleRadius) + windowSize.x / 2;
			const int pointY = int(sin(kl::convert::toRadians(pointAngle * pointCounter + 180)) * circleRadius) + windowSize.y / 2;
			point = kl::int2(pointX, pointY);
			pointCounter++;
		}

		// Drawing the points
		for (kl::int2& point : circlePoints) {
			frame.drawCircle(point, pointRadius, pointColor, true);
		}

		// Drawing the lines
		pointCounter = 0;
		for (kl::int2& point : circlePoints) {
			int secondPoint = abs(int(pointCounter++ * m) % n);
			frame.drawLine(point, circlePoints[secondPoint], lineColor);
		}

		// Displaying
		window.drawImage(frame);

		// Incrementing m
		m += mIncrement * kl::time::interval();
		window.setTitle("Times Table -> n: " + std::to_string(n) + " m: " + std::to_string(m));
	};

	// Creating the window
	window.startNew(windowSize, "Times Table", false, true);
}
