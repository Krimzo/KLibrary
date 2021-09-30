#pragma once
#include "KrimzLib/types.h"


namespace kl {
	// Library constants
	namespace constant {
		// Ints
		const int ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
		const int ScreenHeight = GetSystemMetrics(SM_CYSCREEN);

		// Doubles
		const double pi = 3.141592653589f;
		const double toRadians = pi / 180.0f;
		const double toDegrees = 180.0f / pi;

		// Colors
		const color colorBlack = { 0, 0, 0, 255 };
		const colorf colorfBlack = { 0.00f, 0.00f, 0.00f, 1.00f };
		const color colorWhite = { 200, 200, 200, 255 };
		const colorf colorfWhite = { 0.78f, 0.78f, 0.78f, 1.00f };
		const color colorGray = { 70, 70, 70, 255 };
		const colorf colorfGray = { 0.27f, 0.27f, 0.27f, 1.00f };
		const color colorRed = { 200, 0, 0, 255 };
		const colorf colorfRed = { 0.78f, 0.00f, 0.00f, 1.00f };
		const color colorGreen = { 0, 200, 0, 255 };
		const colorf colorfGreen = { 0.00f, 0.78f, 0.00f, 1.00f };
		const color colorBlue = { 0, 0, 200, 255 };
		const colorf colorfBlue = { 0.00f, 0.00f, 0.78f, 1.00f };
		const color colorCyan = { 32, 178, 170, 255 };
		const colorf colorfCyan = { 0.13f, 0.70f, 0.67f, 1.00f };
		const color colorPurple = { 200, 0, 200, 255 };
		const colorf colorfPurple = { 0.78f, 0.00f, 0.78f, 1.00f };
		const color colorYellow = { 200, 200, 0, 255 };
		const colorf colorfYellow = { 0.78f, 0.78f, 0.00f, 1.00f };
		const color colorOrange = { 255, 140, 0, 255 };
		const colorf colorfOrange = { 1.00f, 0.55f, 0.00f, 1.00f };
		const color colorMagenta = { 153, 0, 153, 255 };
		const colorf colorfMagenta = { 0.60f, 0.00f, 0.60f, 1.00f };
		const color colorCrimson = { 102, 0, 0, 255 };
		const colorf colorfCrimson = { 0.40f, 0.00f, 0.00f, 1.00f };
		const color colorSnow = { 255, 255, 255, 255 };
		const colorf colorfSnow = { 1.00f, 1.00f, 1.00f, 1.00f };
		const color colorSapphire = { 0, 103, 165, 255 };
		const colorf colorfSapphire = { 0.00f, 0.40f, 0.65f, 1.00f };
		const color colorWheat = { 245, 222, 179, 255 };
		const colorf colorfWheat = { 0.96f, 0.87f, 0.70f, 1.00f };
	}
}
