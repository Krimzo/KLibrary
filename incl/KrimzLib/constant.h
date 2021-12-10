#pragma once


namespace kl {
	namespace constant {
		// Ints
		namespace ints {
			const int ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
			const int ScreenHeight = GetSystemMetrics(SM_CYSCREEN);
		}

		// Floats
		namespace floats {
			const float toFloatColor = 1.0f / 255;
		}

		// Doubles
		namespace doubles {
			const double pi = 3.14159265358979;
			const double toRadians = pi / 180;
			const double toDegrees = 180 / pi;
		}

		// Colors
		namespace colors {
			const kl::color black(0, 0, 0);
			const kl::color white(255, 255, 255);
			const kl::color gray(30, 30, 30);
			const kl::color red(255, 0, 0);
			const kl::color green(0, 255, 0);
			const kl::color blue(0, 0, 255);
			const kl::color cyan(32, 178, 170);
			const kl::color purple(220, 0, 220);
			const kl::color yellow(220, 220, 0);
			const kl::color orange(255, 140, 0);
			const kl::color magenta(153, 0, 153);
			const kl::color crimson(102, 0, 0);
			const kl::color wheat(245, 222, 179);
		}
	}
}
