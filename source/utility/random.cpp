#include "utility/random.h"


namespace _ignore {
	static const int _dontcare = [] {
		srand((unsigned int) std::time(nullptr));
		return 0;
	}();
}


bool kl::random::BOOL() {
	return rand() % 2;
}

byte kl::random::BYTE() {
	return byte(rand() % 256);
}
kl::color kl::random::COLOR(bool gray) {
	if (gray) {
		const byte randGray = kl::random::BYTE();
		return kl::color(randGray, randGray, randGray);
	}
	return kl::color(kl::random::BYTE(), kl::random::BYTE(), kl::random::BYTE());
}

int kl::random::INT(int startInclusive, int endExclusive) {
	return rand() % (endExclusive - startInclusive) + startInclusive;
}
int kl::random::INT(int endExclusive) {
	return INT(0, endExclusive);
}

float kl::random::FLOAT(float startInclusive, float endInclusive) {
	return INT(int(startInclusive * 1000.0f), int(endInclusive * 1000.0f) + 1) * 0.001f;
}
float kl::random::FLOAT(float endInclusive) {
	return FLOAT(0.0f, endInclusive);
}
float kl::random::FLOAT() {
	return FLOAT(0.0f, 1.0f);
}

char kl::random::CHAR(bool upperCase) {
	if (upperCase) {
		return char(kl::random::INT(65, 91));
	}
	return char(kl::random::INT(97, 123));
}
std::string kl::random::STRING(uint length) {
	std::stringstream ss;
	for (uint i = 0; i < length; i++) {
		ss << kl::random::CHAR(kl::random::BOOL());
	}
	return ss.str();
}
