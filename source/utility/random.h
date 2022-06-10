#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <windows.h>

#include "math/math.h"


namespace kl {
	namespace random {
		bool BOOL();

		byte BYTE();
		kl::color COLOR(bool gray = false);

		int INT(int startInclusive, int endExclusive);
		int INT(int endExclusive);

		float FLOAT(float startInclusive, float endInclusive);
		float FLOAT(float endInclusive);

		char CHAR(bool upper = false);
		std::string STRING(uint len);
	};
}
