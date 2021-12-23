#pragma once


namespace kl {
    struct size {
		int width;
		int height;

		// Constructors
		size() {
			width = 0;
			height = 0;
		}
		size(int width, int height) {
			this->width = width;
			this->height = height;
		}

		// Operator overloading
		bool operator == (kl::size obj) {
			return (width == obj.width && height == obj.height);
		}
		bool operator != (kl::size obj) {
			return (width != obj.width || height != obj.height);
		}

		// Prints the size
		void print() {
			printf("%d %d\n", width, height);
		}
	};
}