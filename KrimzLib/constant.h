#pragma once


namespace kl {
	// Library constants
	namespace constant {
		// Ints
		const int ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
		const int ScreenHeight = GetSystemMetrics(SM_CYSCREEN);

		// Doubles
		const double pi = 3.14159265358979;

		// Colors
		const color colorBlack = { 0, 0, 0 };
		const color colorWhite = { 255, 255, 255 };
		const color colorGray = { 80, 80, 80 };
		const color colorDarkGray = { 30, 30, 30 };
		const color colorRed = { 255, 0, 0 };
		const color colorGreen = { 0, 255, 0 };
		const color colorBlue = { 0, 0, 255 };
		const color colorCyan = { 32, 178, 170 };
		const color colorPurple = { 220, 0, 220 };
		const color colorYellow = { 220, 220, 0 };
		const color colorOrange = { 255, 140, 0 };
		const color colorMagenta = { 153, 0, 153 };
		const color colorCrimson = { 102, 0, 0 };
		const color colorSapphire = { 0, 103, 165 };
		const color colorWheat = { 245, 222, 179 };
	}
}
