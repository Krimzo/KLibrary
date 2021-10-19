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

		// Returns a vector with random integers
		static std::vector<int> IntVector(int startInclusive, int endExclusive, int vectorSize) {
			std::vector<int> tempVec(vectorSize);
			for (int i = 0; i < vectorSize; i++)
				tempVec[i] = Int(startInclusive, endExclusive);
			return tempVec;
		}

		// Returns a random color
		static color Color(bool randomAlpha = false) {
			return { (byte)Int(0, 256), (byte)Int(0, 256), (byte)Int(0, 256), randomAlpha ? (byte)Int(0, 256) : (byte)255 };
		}
	};
}
