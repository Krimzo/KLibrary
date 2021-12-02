#include "KrimzLib.h"


int main() {
	// Time table properties
	int n = 200;
	double m = 0;
	double mIncrement = 0.1;

	// Circle properties
	double radiusBias = 25;
	kl::color circleColor(220, 220, 220);

	// Point properties
	double pointRadius = 3;
	kl::color pointColor(175, 20, 55);

	// Line properties
	kl::color lineColor(20, 175, 120);

	// Window properties
	kl::size windowSize(1600, 900);


	// Window
	kl::window window;
	kl::image frame(windowSize, kl::constant::colors::gray);
	window.WindowUpdate = [&]() {
		// Clearing the frame
		frame.FillSolid(kl::constant::colors::gray);

		// Circle position and size calculations
		kl::point circleCenter(windowSize.width / 2, windowSize.height / 2);
		double circleRadius = std::min(windowSize.width / 2.0, windowSize.height / 2.0) - radiusBias;

		// Drawing the circle
		frame.DrawCircle(circleCenter, circleRadius, circleColor);

		// Calculating the points
		double pointAngle = 360.0 / n;
		kl::array<kl::point> circlePoints(n);
		int pointCounter = 0;
		circlePoints.RunOnEach([&](kl::point* p) {
			int pointX = int(cos((pointAngle * pointCounter + 180) * kl::constant::doubles::toRadians) * circleRadius) + windowSize.width / 2;
			int pointY = int(sin((pointAngle * pointCounter + 180) * kl::constant::doubles::toRadians) * circleRadius) + windowSize.height / 2;
			*p = kl::point(pointX, pointY);
			pointCounter++;
		});

		// Drawing the points
		circlePoints.RunOnEach([&](kl::point* p) {
			frame.DrawCircle(*p, pointRadius, pointColor, true);
		});

		// Drawing the lines
		pointCounter = 0;
		circlePoints.RunOnEach([&](kl::point* p) {
			int secondPoint = abs(int(pointCounter++ * m) % n);
			frame.DrawLine(*p, circlePoints[secondPoint], lineColor);
		});

		// Displaying
		window.RenderImage(frame);

		// Incrementing m
		m += mIncrement * kl::time::StaticGetElapsed();
		window.SetTitle("Times Table -> n: " + std::to_string(n) + " m: " + std::to_string(m));
	};


	// Creating the window
	window.StartNew(windowSize, "Times Table", false, true);

	return 0;
}
