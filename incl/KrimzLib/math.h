#pragma once


namespace kl {
	namespace math {
		// Returns x from the given y of the line that goes through points a and b
		float lineX(kl::vec2 a, kl::vec2 b, float y) {
			return ((y - a.y) * (b.x - a.x)) / (b.y - a.y) + a.x;
		}

		// Returns y from the given x of the line that goes through points a and b
		float lineY(kl::vec2 a, kl::vec2 b, float x) {
			return ((b.y - a.y) * (x - a.x)) / (b.x - a.x) + a.y;
		}

		// Checks if the given number is prime
		bool isPrime(uint64_t num) {
			if (num == 0 || num == 1) {
				return false;
			}

			uint64_t half = num / 2;
			for (uint64_t i = 2; i <= half; i++) {
				if (num % i == 0) {
					return false;
				}
			}
			return true;
		}

		// Fast inverse sqrt
		float invSqrt(float number) {
			int i = 0x5f3759df - (*(int*)&number >> 1);
			float y = *(float*)&i;
			y = y * (1.5f - ((number * 0.5f) * y * y));
			return y;
		}
	};
}
