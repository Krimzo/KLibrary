#pragma once


namespace kl {
	namespace constant {
		// Ints
		namespace ints {
			const int ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
			const int ScreenHeight = GetSystemMetrics(SM_CYSCREEN);
		}

		// Doubles
		namespace doubles {
			const double pi = 3.14159265358979;
			const double toRadians = pi / 180;
			const double toDegrees = 180 / pi;
		}

		// Colors
		namespace colors {
			const color black = { 0, 0, 0 };
			const color white = { 255, 255, 255 };
			const color gray = { 30, 30, 30 };
			const color red = { 255, 0, 0 };
			const color green = { 0, 255, 0 };
			const color blue = { 0, 0, 255 };
			const color cyan = { 32, 178, 170 };
			const color purple = { 220, 0, 220 };
			const color yellow = { 220, 220, 0 };
			const color orange = { 255, 140, 0 };
			const color magenta = { 153, 0, 153 };
			const color crimson = { 102, 0, 0 };
			const color wheat = { 245, 222, 179 };
		}
	}
}
