#pragma once


namespace kl {
    struct vec2 {
		float x;
		float y;

		// Constructors
		vec2() {
			x = 0;
			y = 0;
		}
		vec2(float x, float y) {
			this->x = x;
			this->y = y;
		}
		vec2(kl::point p) {
			x = float(p.x);
			y = float(p.y);
		}
		vec2(kl::point a, kl::point b) {
			x = float(b.x - a.x);
			y = float(b.y - a.y);
		}

		// Operator overloading
		kl::vec2 operator + (kl::vec2 obj) {
			return kl::vec2(x + obj.x, y + obj.y);
		}
		kl::vec2 operator - (kl::vec2 obj) {
			return kl::vec2(x - obj.x, y - obj.y);
		}
		kl::vec2 operator * (float a) {
			return kl::vec2(x * a, y * a);
		}
		kl::vec2 operator / (float a) {
			return kl::vec2(x / a, y / a);
		}
		bool operator == (kl::vec2 obj) {
			return (x == obj.x && y == obj.y);
		}
		bool operator != (kl::vec2 obj) {
			return !operator==(obj);
		}

		// Returns the vectors length
		float length() {
			return sqrt(x * x + y * y);
		}

		// Retruns a normalized vector
		kl::vec2 normalize() {
			float vecLen = 1 / length();
			return kl::vec2(x * vecLen, y * vecLen);
		}

		// Returns the dot product
		float dot(vec2 a) {
			return x * a.x + y * a.y;
		}

		// Prints the data to the console
		void print() {
			printf("%.2f %.2f\n", x, y);
		}
	};   
}
