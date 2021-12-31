#pragma once


namespace kl {
	class random {
	public:
		// Returns a random bool
		static bool BOOL() {
			return rand() % 2;
		}

		// Returns a random byte
		static kl::byte BYTE() {
			return kl::byte(rand() % 256);
		}

		// Returns a random integer
		static int INT(int startInclusive, int endExclusive) {
			return rand() % (endExclusive - startInclusive) + startInclusive;
		}
		static int INT(int endExclusive) {
			return INT(0, endExclusive);
		}

		// Returns a random float
		static float FLOAT(float startInclusive, float endExclusive) {
			return startInclusive + float(rand()) / float(RAND_MAX / (endExclusive - startInclusive));
		}
		static float FLOAT(float endExclusive) {
			return FLOAT(0, endExclusive);
		}

		// Returns a random letter
		static char LETTER(bool upperCase = false) {
			if (upperCase) {
				return char(kl::random::INT(65, 91));
			}
			return char(kl::random::INT(97, 123));
		}

		// Returns a random string
		static std::string STRING(int len) {
			std::stringstream ss;
			for (int i = 0; i < len; i++) {
				ss << kl::random::LETTER(kl::random::BOOL());
			}
			return ss.str();
		}

		// Returns a random color
		static kl::color getColor(bool grayScaled = false) {
			if (grayScaled) {
				kl::byte randGray = kl::random::BYTE();
				return kl::color(randGray, randGray, randGray);
			}
			return kl::color(kl::random::BYTE(), kl::random::BYTE(), kl::random::BYTE());
		}

	protected:
		// Sets the seed for random number generation
		static void newSeed() {
			srand((unsigned int)std::time(nullptr));
		}
	};
}
