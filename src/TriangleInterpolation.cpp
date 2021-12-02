#include "KrimzLib.h"


kl::vec3 calcBaryWeights(kl::point, kl::point, kl::point, kl::point, double);

int main() {
	// Window size
	const kl::size windowSize(800, 800);

	// Scanline height
	const kl::uint32 scanHeight = 100;

	// Triangle points
	kl::point A(0, (windowSize.height - 1) / 3);
	kl::point B((windowSize.width - 1) / 3, windowSize.height - 1);
	kl::point C(windowSize.width - 1, (windowSize.height - 1) * 0.667);
	kl::point D((windowSize.width - 1) * 0.667, 0);

	// Point colors
	kl::color cA = kl::constant::colors::red;
	kl::color cB = kl::constant::colors::black;
	kl::color cC = kl::constant::colors::blue;
	kl::color cD = kl::constant::colors::white;

	// Calculating the interpolation constant
	const double baryConst1 = 1.0 / ((B.y - C.y) * (A.x - C.x) + (C.x - B.x) * (A.y - C.y));
	const double baryConst2 = 1.0 / ((C.y - D.y) * (A.x - D.x) + (D.x - C.x) * (A.y - D.y));

	// Window creation
	kl::window window;
	kl::image frame(windowSize);

	// Window update setup
	int i = 0;
	window.WindowUpdate = [&]() {
		// Getting x and y coords
		int x = i % windowSize.width;
		int y = (i / windowSize.width) * scanHeight;

		for (int yIncr = 0; yIncr < scanHeight; yIncr++) {
			// Pixel color
			kl::color pixel;

			// Calculating the interpolation weights
			kl::vec3 baryWeights1 = calcBaryWeights(A, B, C, kl::point(x, y + yIncr), baryConst1);
			kl::vec3 baryWeights2 = calcBaryWeights(A, C, D, kl::point(x, y + yIncr), baryConst2);

			// Checking if the point is inside first triangle
			if (!(baryWeights1.x < 0 || baryWeights1.y < 0 || baryWeights1.z < 0)) {
				// Interpolating the color
				const kl::byte r(cA.r * baryWeights1.x + cB.r * baryWeights1.y + cC.r * baryWeights1.z);
				const kl::byte g(cA.g * baryWeights1.x + cB.g * baryWeights1.y + cC.g * baryWeights1.z);
				const kl::byte b(cA.b * baryWeights1.x + cB.b * baryWeights1.y + cC.b * baryWeights1.z);

				// Saving to pixel
				pixel = kl::color(r, g, b);
			}
			// Checking if the point is inside second triangle
			else if (!(baryWeights2.x < 0 || baryWeights2.y < 0 || baryWeights2.z < 0)) {
				// Interpolating the color
				const kl::byte r(cA.r * baryWeights2.x + cC.r * baryWeights2.y + cD.r * baryWeights2.z);
				const kl::byte g(cA.g * baryWeights2.x + cC.g * baryWeights2.y + cD.g * baryWeights2.z);
				const kl::byte b(cA.b * baryWeights2.x + cC.b * baryWeights2.y + cD.b * baryWeights2.z);

				// Saving to pixel
				pixel = kl::color(r, g, b);
			}
			// Drawing the background
			else {
				pixel = kl::constant::colors::gray;
			}

			// Setting the pixel
			frame.SetPixel(kl::point(x, y + yIncr), pixel);

			// Setting the future pixel
			frame.SetPixel(kl::point(x + 1, y + yIncr), kl::constant::colors::white);
		}
		
		// Drawing the frame
		window.RenderImage(frame);

		// Updating the title info
		window.SetTitle(std::to_string(i * scanHeight) + " of " + std::to_string(windowSize.width * windowSize.height));

		// Updating the counter
		i++;

		// Checking the i
		if (x == (windowSize.width - 1) && (y + scanHeight - 1) >= (windowSize.height - 1)) {
			window.SetTitle("Finished!");
			window.WindowUpdate = []() {};
		}
	};

	// Window start
	window.StartNew(windowSize, "Times Table", false, true);


	return 0;
}


// Calculates and returns the 3 barycentric weights of a triangle and a point
kl::vec3 calcBaryWeights(kl::point A, kl::point B, kl::point C, kl::point P, double baryConst) {
	double baryWeight1 = ((B.y - C.y) * (P.x - C.x) + (C.x - B.x) * (P.y - C.y)) * baryConst;
	double baryWeight2 = ((C.y - A.y) * (P.x - C.x) + (A.x - C.x) * (P.y - C.y)) * baryConst;
	return kl::vec3(baryWeight1, baryWeight2, 1 - baryWeight1 - baryWeight2);
}
