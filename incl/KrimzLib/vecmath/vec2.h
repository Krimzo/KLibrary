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
		vec2(float a) {
			x = a;
			y = a;
		}
		vec2(float x, float y) {
			this->x = x;
			this->y = y;
		}
		vec2(kl::ivec2 p) {
			x = (float)p.x;
			y = (float)p.y;
		}
		vec2(kl::ivec2 a, kl::ivec2 b) {
			x = float(b.x - a.x);
			y = float(b.y - a.y);
		}

		// Operator overloading
		kl::vec2 operator + (kl::vec2 obj) {
			return kl::vec2(x + obj.x, y + obj.y);
		}
		void operator += (kl::vec2 obj) {
			x += obj.x; y += obj.y;
		}
		kl::vec2 operator - (kl::vec2 obj) {
			return kl::vec2(x - obj.x, y - obj.y);
		}
		void operator -= (kl::vec2 obj) {
			x -= obj.x; y -= obj.y;
		}
		kl::vec2 operator * (float a) {
			return kl::vec2(x * a, y * a);
		}
		void operator *= (float a) {
			x *= a; y *= a;
		}
		kl::vec2 operator / (float a) {
			return operator*(1 / a);
		}
		void operator /= (float a) {
			operator*=(1 / a);
		}
		bool operator == (kl::vec2 obj) {
			return (x == obj.x && y == obj.y);
		}
		bool operator != (kl::vec2 obj) {
			return !operator==(obj);
		}

		// Returns a negated vector
		kl::vec2 negate() {
			return operator*(-1);
		}

		// Returns the vectors length
		float length() {
			return sqrt(x * x + y * y);
		}

		// Retruns a normalized vector
		kl::vec2 normalize() {
			return operator/(length());
			//return operator*(kl::math::invSqrt(x * x + y * y));
		}

		// Returns the dot product
		float dot(vec2 a) {
			return x * a.x + y * a.y;
		}

		// Returns the angle between the given vector and self
		float angle(kl::vec2 a) {
			return acos(this->normalize().dot(a.normalize())) * 57.2957795131f;
		}

		// Returns a rotated vector around the given point
		kl::vec2 rotate(float angle) {
			const float sinA = sin(angle * 0.01745329251f);
			const float cosA = cos(angle * 0.01745329251f);
			return kl::vec2(cosA * x - sinA * y, sinA * x + cosA * y);
		}

		// Prints the data to the console
		void print() {
			printf("% .2f % .2f\n", x, y);
		}
	};   
}
