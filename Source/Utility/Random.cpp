#include "Utility/Random.h"


namespace _ignore {
	static const int _dontcare = [] {
		srand(kl::uint(time(nullptr)));
		return 0;
	}();
}

bool kl::Random::BOOL() {
	return rand() % 2;
}

byte kl::Random::BYTE() {
	return byte(rand() % 256);
}

kl::Color kl::Random::COLOR(bool gray) {
	if (gray) {
		const byte randGray = BYTE();
		return { randGray, randGray, randGray };
	}
	return { BYTE(), BYTE(), BYTE() };
}

int kl::Random::INT(int startInclusive, int endExclusive) {
	return rand() % (endExclusive - startInclusive) + startInclusive;
}

int kl::Random::INT(int endExclusive) {
	return INT(0, endExclusive);
}

float kl::Random::FLOAT(float startInclusive, float endInclusive) {
	return FLOAT(endInclusive - startInclusive) + startInclusive;
}

float kl::Random::FLOAT(float endInclusive) {
	return FLOAT() * endInclusive;
}

float kl::Random::FLOAT() {
	return rand() / float(RAND_MAX);
}

char kl::Random::CHAR(bool upperCase) {
	if (upperCase) {
		return char(INT(65, 91));
	}
	return char(INT(97, 123));
}

kl::String kl::Random::STRING(uint length) {
	StringStream stream;
	for (uint i = 0; i < length; i++) {
		stream << CHAR(BOOL());
	}
	return stream.str();
}
