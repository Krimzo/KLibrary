#pragma once


namespace kl {
	class random {
	public:
		// Returns a random bool
		static bool Bool() {
			return rand() % 2;
		}

		// Returns a random byte
		static kl::byte Byte() {
			return kl::byte(rand() % 256);
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

		// Returns a random letter
		static char Letter(bool upperCase = false) {
			if (upperCase) {
				return (char)random::Int(65, 91);
			}
			return (char)random::Int(97, 123);
		}

		// Returns a random string
		static std::string String(kl::uint32 size) {
			std::stringstream ss;
			for (kl::uint32 i = 0; i < size; i++) {
				ss << random::Letter(random::Bool());
			}
			return ss.str();
		}

		// Returns a random color
		static kl::color Color() {
			return { random::Byte(), random::Byte(), random::Byte() };
		}
		static kl::colord Colord() {
			return random::Color();
		}

	protected:
		// Sets the seed for random number generation
		static void NewSeed() {
			srand((unsigned)std::time(nullptr));
		}
	};
}
