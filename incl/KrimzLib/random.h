#pragma once


namespace kl {
	class random {
	public:
		// Sets the seed for random number generation
		static void SetSeed() {
			srand((unsigned)std::time(NULL));
		}

		// Returns a random integer
		static int Int(int startInclusive, int endExclusive) {
			return rand() % (endExclusive - startInclusive) + startInclusive;
		}

		// Returns a random color
		static color Color() {
			return { (byte)Int(0, 256), (byte)Int(0, 256), (byte)Int(0, 256) };
		}
		static colorf Colorf() {
			return color((byte)Int(0, 256), (byte)Int(0, 256), (byte)Int(0, 256));
		}
		static colord Colord() {
			return color((byte)Int(0, 256), (byte)Int(0, 256), (byte)Int(0, 256));
		}
	};
}
