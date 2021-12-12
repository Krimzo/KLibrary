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

		// Operator overloading
		kl::vec4 operator + (kl::vec4 obj) {
			return kl::vec4(x + obj.x, y + obj.y, z + obj.z, w);
		}
		kl::vec4 operator - (kl::vec4 obj) {
			return kl::vec4(x - obj.x, y - obj.y, z - obj.z, w);
		}
		kl::vec4 operator * (float a) {
			return kl::vec4(x * a, y * a, z * a, w);
		}
		kl::vec4 operator / (float a) {
			return kl::vec4(x / a, y / a, z / a, w);
		}
		bool operator == (kl::vec4 obj) {
			return (x == obj.x && y == obj.y && z == obj.z && w == obj.w);
		}
		bool operator != (kl::vec4 obj) {
			return (x != obj.x || y != obj.y || z != obj.z || w != obj.w);
		}

		// Returns the vectors length
		float length() {
			return sqrt(x * x + y * y + z * z);
		}

		// Retruns a normalized vector
		kl::vec4 normalize() {
			float vecLen = 1 / length();
			return kl::vec4(x * vecLen, y * vecLen, z * vecLen, w);
		}

		// Returns the dot product
		float dot(kl::vec4 a) {
			return x * a.x + y * a.y + z * a.z;
		}

		// Returns the dot product
		kl::vec4 cross(kl::vec4 a) {
			return kl::vec4(y * a.z - z * a.y, z * a.x - x * a.z, x * a.y - y * a.x, w);
		}

		// Prints the data to the console
		void print() {
			printf("%f %f %f %f\n", x, y, z, w);
		}
	};
}
