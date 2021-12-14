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
		static std::string getString(kl::uint32 size) {
			std::stringstream ss;
			for (kl::uint32 i = 0; i < size; i++) {
				ss << kl::random::getLetter(kl::random::getBool());
			}
			return ss.str();
		}

		// Returns a random color
		static kl::color getColor() {
			return kl::color(kl::random::getByte(), kl::random::getByte(), kl::random::getByte());
		}

	protected:
		// Sets the seed for random number generation
		static void newSeed() {
			srand(unsigned(std::time(nullptr)));
		}
	};
}
