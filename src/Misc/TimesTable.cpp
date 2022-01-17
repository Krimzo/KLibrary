#include "KrimzLib.hpp"


int main() {
	// Time table properties
	int n = 200;
	float m = 0;
	float mIncrement = 0.1;

	// Circle properties
	float radiusBias = 25;
	kl::color circleColor(220, 220, 220);

	// Point properties
	float pointRadius = 3;
	kl::color pointColor(175, 20, 55);

	// Line properties
	kl::color lineColor(20, 175, 120);

	// Window properties
	kl::ivec2 windowSize(1600, 900);


	// Window
	kl::window window;
	kl::image frame(windowSize, kl::colors::gray);
	window.update = [&]() {
		// Clearing the frame
		frame.fillSolid(kl::colors::gray);

		// Circle position and size calculations
		kl::ivec2 circleCenter(windowSize.x / 2, windowSize.y / 2);
		float circleRadius = std::min(windowSize.x / 2.0, windowSize.y / 2.0) - radiusBias;

		// Drawing the circle
		frame.drawCircle(circleCenter, circleRadius, circleColor);

		// Calculating the points
		float pointAngle = 360.0 / n;
		std::vector<kl::ivec2> circlePoints(n);
		int pointCounter = 0;
		for (kl::ivec2& point : circlePoints) {
			int pointX = int(cos(kl::convert::toRadians(pointAngle * pointCounter + 180)) * circleRadius) + windowSize.x / 2;
			int pointY = int(sin(kl::convert::toRadians(pointAngle * pointCounter + 180)) * circleRadius) + windowSize.y / 2;
			point = kl::ivec2(pointX, pointY);
			pointCounter++;
		}

		// Drawing the points
		for (kl::ivec2& point : circlePoints) {
			frame.drawCircle(point, pointRadius, pointColor, true);
		}

		// Drawing the lines
		pointCounter = 0;
		for (kl::ivec2& point : circlePoints) {
			int secondPoint = abs(int(pointCounter++ * m) % n);
			frame.drawLine(point, circlePoints[secondPoint], lineColor);
		}

		// Displaying
		window.drawImage(frame);

		// Incrementing m
		m += mIncrement * kl::time::getInterval();
		window.setTitle("Times Table -> n: " + std::to_string(n) + " m: " + std::to_string(m));
	};

	// Creating the window
	kl::time::getInterval();
	window.startNew(windowSize, "Times Table", false, true);

	return 0;
}
