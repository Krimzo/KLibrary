#pragma once


namespace kl {
	class random {
	public:
		// Returns a random bool
		static bool getBool() {
			return rand() % 2;
		}

		// Returns a random byte
		static kl::byte getByte() {
			return kl::byte(rand() % 256);
		}

		// Returns a random integer
		static int getInt(int startInclusive, int endExclusive) {
			return rand() % (endExclusive - startInclusive) + startInclusive;
		}
		static int getInt(int endExclusive) {
			return getInt(0, endExclusive);
		}

		// Returns a random float
		static float getFloat(float startInclusive, float endExclusive) {
			return startInclusive + float(rand()) / float(RAND_MAX / (endExclusive - startInclusive));
		}
		static float getFloat(float endExclusive) {
			return getFloat(0, endExclusive);
		}

		// Returns a random letter
		static char getLetter(bool upperCase = false) {
			if (upperCase) {
				return char(kl::random::getInt(65, 91));
			}
			return char(kl::random::getInt(97, 123));
		}

		// Returns a random string
		static std::string getString(int len) {
			std::stringstream ss;
			for (int i = 0; i < len; i++) {
				ss << kl::random::getLetter(kl::random::getBool());
			}
			return ss.str();
		}

		// Returns a random color
		static kl::color getColor(bool grayScaled = false) {
			if (grayScaled) {
				kl::byte randGray = kl::random::getByte();
				return kl::color(randGray, randGray, randGray);
			}
			return kl::color(kl::random::getByte(), kl::random::getByte(), kl::random::getByte());
		}

	protected:
		// Sets the seed for random number generation
		static void newSeed() {
			srand((unsigned int)std::time(nullptr));
		}
	};
}
