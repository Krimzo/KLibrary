#pragma once


namespace kl {
	class random {
	public:
		// Sets the seed for random number generation
		static void NewSeed() {
			srand((unsigned)std::time(NULL));
		}

		// Returns a random bool
		static bool Bool() {
			return rand() % 2;
		}

		// Returns a random integer
		static int Int(int startInclusive, int endExclusive) {
			return rand() % (endExclusive - startInclusive) + startInclusive;
		}

		// Returns a random float
		static float Float(float startInclusive, float endExclusive) {
			return startInclusive + (float)rand() / float(RAND_MAX / (endExclusive - startInclusive));
		}

		// Returns a random double
		static double Double(double startInclusive, double endExclusive) {
			return startInclusive + (double)rand() / double(RAND_MAX / (endExclusive - startInclusive));
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
