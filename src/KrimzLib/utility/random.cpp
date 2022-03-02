#include "KrimzLib/utility/random.h"


// Random seed init
namespace _ignore {
	const int _dontcare = [] {
		srand((unsigned int)std::time(nullptr));
		return 0;
	}();
}

// Returns a random bool
bool kl::random::BOOL() {
	return rand() % 2;
}

// Returns a random byte
byte kl::random::BYTE() {
	return byte(rand() % 256);
}

// Returns a random integer
int kl::random::INT(int startInclusive, int endExclusive) {
	return rand() % (endExclusive - startInclusive) + startInclusive;
}
int kl::random::INT(int endExclusive) {
	return INT(0, endExclusive);
}

// Returns a random float
float kl::random::FLOAT(float startInclusive, float endInclusive) {
	return INT(int(startInclusive * 1000), int(endInclusive * 1000) + 1) * 0.001f;
}
float kl::random::FLOAT(float endInclusive) {
	return FLOAT(0, endInclusive);
}

// Returns a random letter
char kl::random::LETTER(bool upperCase) {
	if (upperCase) {
		return char(kl::random::INT(65, 91));
	}
	return char(kl::random::INT(97, 123));
}

// Returns a random string
std::string kl::random::STRING(int len) {
	std::stringstream ss;
	for (int i = 0; i < len; i++) {
		ss << kl::random::LETTER(kl::random::BOOL());
	}
	return ss.str();
}

// Returns a random color
kl::color kl::random::COLOR(bool grayScaled) {
	if (grayScaled) {
		byte randGray = kl::random::BYTE();
		return kl::color(randGray, randGray, randGray);
	}
	return kl::color(kl::random::BYTE(), kl::random::BYTE(), kl::random::BYTE(), kl::random::BYTE());
}
