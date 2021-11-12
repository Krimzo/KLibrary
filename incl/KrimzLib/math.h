#pragma once


namespace kl {
	class math {
	public:
		// Returns a dot product of 2 given vectors
		static double VectorDotProd(kl::vec2 a, kl::vec2 b) {
			return a.x * b.x + a.y * b.y;
		}
		static double VectorDotProd(kl::vec3 a, kl::vec3 b) {
			return a.x * b.x + a.y * b.y + a.z * b.z;
		}

		// Returns a cross product of 2 given vectors
		static kl::vec3 VectorCrossProd(kl::vec3 a, kl::vec3 b) {
			return { a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x };
		}

		// Returns x from the given y of the line that goes through points a and b
		static double XofLine(kl::vec2 a, kl::vec2 b, double y) {
			return ((y - a.y) * (b.x - a.x)) / (b.y - a.y) + a.x;
		}

		// Returns y from the given x of the line that goes through points a and b
		static double YofLine(kl::vec2 a, kl::vec2 b, double x) {
			return ((b.y - a.y) * (x - a.x)) / (b.x - a.x) + a.y;
		}

		// Checks if the given number is prime
		static bool IsPrime(kl::uint64 num) {
			if (num == 0 || num == 1) {
				return false;
			}

			kl::uint64 half = num / 2;
			for (kl::uint64 i = 2; i <= half; i++) {
				if (num % i == 0) {
					return false;
				}
			}
			return true;
		}
	};
}
