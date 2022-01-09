#pragma once 


namespace kl {
	typedef unsigned char byte;
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
	namespace colors {
		static const kl::color black(0, 0, 0);
		static const kl::color white(255, 255, 255);
		static const kl::color gray(50, 50, 50);
		static const kl::color red(255, 0, 0);
		static const kl::color green(0, 255, 0);
		static const kl::color blue(0, 0, 255);
		static const kl::color cyan(30, 180, 170);
		static const kl::color purple(220, 0, 220);
		static const kl::color yellow(220, 220, 0);
		static const kl::color orange(255, 140, 0);
		static const kl::color magenta(155, 0, 155);
		static const kl::color crimson(100, 0, 0);
		static const kl::color wheat(245, 220, 180);
	};
}
