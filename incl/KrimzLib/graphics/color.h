#pragma once 


namespace kl {
    struct color {
		// Had to flip R and B because the winapi uses BGR
		kl::byte b;
		kl::byte g;
		kl::byte r;

		// Constructors
		color() {
			r = 0;
			g = 0;
			b = 0;
		}
		color(kl::byte r, kl::byte g, kl::byte b) {
			this->r = r;
			this->g = g;
			this->b = b;
		}

		// Operator overloading
		bool operator == (kl::color obj) {
			return (r == obj.r && g == obj.g && b == obj.b);
		}
		bool operator != (kl::color obj) {
			return (r != obj.r || g != obj.g || b != obj.b);
		}

		// Prints the color
		void print() {
			printf("%d %d %d\n", r, g, b);
		}
	};
}
