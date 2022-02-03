#pragma once


namespace kl {
	namespace _kl_dont_use_ {
		struct _kl_random_initialization_ {
			_kl_random_initialization_() {
				srand((unsigned int)std::time(nullptr));
			}
		};
		_kl_random_initialization_ _random_init_;
	}

	namespace random {
		// Returns a random bool
		bool BOOL() {
			return rand() % 2;
		}

		// Returns a random byte
		kl::byte BYTE() {
			return kl::byte(rand() % 256);
		}

		// Returns a random integer
		int INT(int startInclusive, int endExclusive) {
			return rand() % (endExclusive - startInclusive) + startInclusive;
		}
		int INT(int endExclusive) {
			return INT(0, endExclusive);
		}

		// Returns a random float
		float FLOAT(float startInclusive, float endInclusive) {
			return INT(int(startInclusive * 1000), int(endInclusive * 1000) + 1) * 0.001f;
		}
		float FLOAT(float endInclusive) {
			return FLOAT(0, endInclusive);
		}

		// Returns a random letter
		char LETTER(bool upperCase = false) {
			if (upperCase) {
				return char(kl::random::INT(65, 91));
			}
			return char(kl::random::INT(97, 123));
		}

		// Returns a random string
		String STRING(int len) {
			std::stringstream ss;
			for (int i = 0; i < len; i++) {
				ss << kl::random::LETTER(kl::random::BOOL());
			}
			return ss.str();
		}

		// Returns a random color
		kl::color COLOR(bool grayScaled = false) {
			if (grayScaled) {
				kl::byte randGray = kl::random::BYTE();
				return kl::color(randGray, randGray, randGray);
			}
			return kl::color(kl::random::BYTE(), kl::random::BYTE(), kl::random::BYTE());
		}
	};
}
