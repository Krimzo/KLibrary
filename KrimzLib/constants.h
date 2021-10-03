#pragma once
#include "KrimzLib/types.h"


namespace kl
{
	// Library constants
	namespace constant
	{
		// Ints
		const int ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
		const int ScreenHeight = GetSystemMetrics(SM_CYSCREEN);

		// Doubles
		const double pi = 3.141592653589f;
		const double toRadians = pi / 180.0f;
		const double toDegrees = 180.0f / pi;

		// Colors
		const color colorBlack = { 0, 0, 0, 255 };
		const color colorWhite = { 255, 255, 255, 255 };
		const color colorGray = { 80, 80, 80, 255 };
		const color colorDarkGray = { 30, 30, 30, 255 };
		const color colorRed = { 255, 0, 0, 255 };
		const color colorGreen = { 0, 255, 0, 255 };
		const color colorBlue = { 0, 0, 255, 255 };
		const color colorCyan = { 32, 178, 170, 255 };
		const color colorPurple = { 220, 0, 220, 255 };
		const color colorYellow = { 220, 220, 0, 255 };
		const color colorOrange = { 255, 140, 0, 255 };
		const color colorMagenta = { 153, 0, 153, 255 };
		const color colorCrimson = { 102, 0, 0, 255 };
		const color colorSnow = { 255, 255, 255, 255 };
		const color colorSapphire = { 0, 103, 165, 255 };
		const color colorWheat = { 245, 222, 179, 255 };
	}
}
