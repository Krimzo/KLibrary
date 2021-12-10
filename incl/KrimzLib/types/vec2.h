#pragma once


namespace kl {
    struct vec2 {
		double x;
		double y;

		// Constructors
		vec2() {
			x = 0;
			y = 0;
		}
		vec2(double x, double y) {
			this->x = x;
			this->y = y;
		}
		vec2(kl::point p) {
			x = double(p.x);
			y = double(p.y);
		}
		vec2(kl::point a, kl::point b) {
			x = double(b.x - a.x);
			y = double(b.y - a.y);
		}

		// Operator overloading
		kl::vec2 operator + (kl::vec2 obj) {
			return kl::vec2(x + obj.x, y + obj.y);
		}
		kl::vec2 operator - (kl::vec2 obj) {
			return kl::vec2(x - obj.x, y - obj.y);
		}
		kl::vec2 operator * (double a) {
			return kl::vec2(x * a, y * a);
		}
		kl::vec2 operator / (double a) {
			return kl::vec2(x / a, y / a);
		}
		bool operator == (kl::vec2 obj) {
			return (x == obj.x && y == obj.y);
		}
		bool operator != (kl::vec2 obj) {
			return (x != obj.x || y != obj.y);
		}

		// Returns the vectors length
		double length() {
			return sqrt(x * x + y * y);
		}

		// Retruns a normalized vector
		kl::vec2 normalize() {
			double vecLen = 1 / length();
			return kl::vec2(x * vecLen, y * vecLen);
		}

		// Returns the dot product
		double dot(vec2 a) {
			return x * a.x + y * a.y;
		}

		// Prints the data to the console
		void print() {
			printf("%lf %lf\n", x, y);
		}
	};   
}
