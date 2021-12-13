#pragma once


namespace kl {
	struct vec4 {
		float x;
		float y;
		float z;
		float w;

		// Constructors
		vec4() {
			x = 0;
			y = 0;
			z = 0;
			w = 0;
		}
		vec4(float x, float y, float z, float w) {
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
		}
		vec4(kl::vec2 v, float z, float w) {
			x = v.x;
			y = v.y;
			this->z = z;
			this->w = w;
		}
		vec4(float x, kl::vec2 v, float w) {
			this->x = x;
			y = v.x;
			z = v.y;
			this->w = w;
		}
		vec4(float x, float y, kl::vec2 v) {
			this->x = x;
			this->y = y;
			z = v.x;
			w = v.y;
		}
		vec4(kl::vec2 v1, kl::vec2 v2) {
			x = v1.x;
			y = v1.y;
			z = v2.x;
			w = v2.y;
		}
		vec4(kl::vec3 v) {
			x = v.x;
			y = v.y;
			z = v.z;
			w = 1;
		}
		vec4(kl::vec3 v, float w) {
			x = v.x;
			y = v.y;
			z = v.z;
			this->w = w;
		}
		vec4(float x, kl::vec3 v) {
			this->x = x;
			y = v.x;
			z = v.y;
			w = v.z;
		}

		// Operator overloading
		kl::vec4 operator + (kl::vec4 obj) {
			return kl::vec4(x + obj.x, y + obj.y, z + obj.z, w + obj.w);
		}
		kl::vec4 operator - (kl::vec4 obj) {
			return kl::vec4(x - obj.x, y - obj.y, z - obj.z, w - obj.w);
		}
		kl::vec4 operator * (float a) {
			return kl::vec4(x * a, y * a, z * a, w * a);
		}
		kl::vec4 operator / (float a) {
			const float rec = 1 / a;
			return kl::vec4(x * rec, y * rec, z * rec, w * rec);
		}
		bool operator == (kl::vec4 obj) {
			return (x == obj.x && y == obj.y && z == obj.z && w == obj.w);
		}
		bool operator != (kl::vec4 obj) {
			return !operator==(obj);
		}

		// Returns the vectors length
		float length() {
			return sqrt(x * x + y * y + z * z);
		}

		// Retruns a normalized vector
		kl::vec4 normalize() {
			const float vecLen = 1 / length();
			return kl::vec4(x * vecLen, y * vecLen, z * vecLen, w);
		}

		// Returns the dot product
		float dot(kl::vec4 a) {
			return x * a.x + y * a.y + z * a.z + w * a.w;
		}

		// Returns the dot product
		kl::vec4 cross(kl::vec4 a) {
			return kl::vec4(y * a.z - z * a.y, z * a.x - x * a.z, x * a.y - y * a.x, w);
		}

		// Prints the data to the console
		void print() {
			printf("%.2f %.2f %.2f %.2f\n", x, y, z, w);
		}
	};
}
