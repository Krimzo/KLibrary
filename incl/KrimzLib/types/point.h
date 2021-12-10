#pragma once 


namespace kl {
    struct point {
		int x;
		int y;

		// Constructors
		point() {
			x = 0;
			y = 0;
		}
		point(int x, int y) {
			this->x = x;
			this->y = y;
		}

		// Operator overloading
		bool operator == (kl::point obj) {
			return (x == obj.x && y == obj.y);
		}
		bool operator != (kl::point obj) {
			return (x != obj.x || y != obj.y);
		}
	};
}