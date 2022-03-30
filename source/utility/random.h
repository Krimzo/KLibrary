#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <windows.h>

#include "color/color.h"


namespace kl {
	namespace random {
		// Returns a random bool
		bool BOOL();

		// Returns a random byte
		byte BYTE();

		// Returns a random integer
		int INT(int startInclusive, int endExclusive);
		int INT(int endExclusive);

		// Returns a random float
		float FLOAT(float startInclusive, float endInclusive);
		float FLOAT(float endInclusive);

		// Returns a random letter
		char LETTER(bool upperCase = false);

		// Returns a random string
		std::string STRING(int len);

		// Returns a random color
		kl::color COLOR(bool grayScaled = false);
	};
}
