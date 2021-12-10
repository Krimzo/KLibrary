#pragma once


namespace kl {
	namespace math {
		// Returns x from the given y of the line that goes through points a and b
		double lineX(kl::vec2 a, kl::vec2 b, double y) {
			return ((y - a.y) * (b.x - a.x)) / (b.y - a.y) + a.x;
		}

		// Returns y from the given x of the line that goes through points a and b
		double lineY(kl::vec2 a, kl::vec2 b, double x) {
			return ((b.y - a.y) * (x - a.x)) / (b.x - a.x) + a.y;
		}

		// Checks if the given number is prime
		bool isPrime(kl::uint64 num) {
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
