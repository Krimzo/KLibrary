#pragma once


namespace kl {
	struct ivec2 {
		int x;
		int y;

		// Constructors
		ivec2() {
			x = 0;
			y = 0;
		}
		ivec2(int x, int y) {
			this->x = x;
			this->y = y;
		}

		// Operator overloading
		kl::ivec2 operator + (kl::ivec2 obj) {
			return kl::ivec2(x + obj.x, y + obj.y);
		}
		void operator += (kl::ivec2 obj) {
			x += obj.x; y += obj.y;
		}
		kl::ivec2 operator - (kl::ivec2 obj) {
			return kl::ivec2(x - obj.x, y - obj.y);
		}
		void operator -= (kl::ivec2 obj) {
			x -= obj.x; y -= obj.y;
		}
		kl::ivec2 operator * (int a) {
			return kl::ivec2(x * a, y * a);
		}
		void operator *= (int a) {
			x *= a; y *= a;
		}
		kl::ivec2 operator / (int a) {
			return kl::ivec2(x / a, y / a);
		}
		void operator /= (float a) {
			x /= a; y /= a;
		}
		bool operator == (kl::ivec2 obj) {
			return (x == obj.x && y == obj.y);
		}
		bool operator != (kl::ivec2 obj) {
			return !operator==(obj);
		}

		// Prints the data to the console
		void print() {
			printf("%d %d\n", x, y);
		}
	};
}
