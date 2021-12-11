#pragma once


namespace kl {
    struct vec3 {
		float x;
		float y;
		float z;

		// Constructors
		vec3() {
			x = 0;
			y = 0;
			z = 0;
		}
		vec3(float x, float y, float z) {
			this->x = x;
			this->y = y;
			this->z = z;
		}

		// Operator overloading
		kl::vec3 operator + (kl::vec3 obj) {
			return kl::vec3(x + obj.x, y + obj.y, z + obj.z);
		}
		kl::vec3 operator - (kl::vec3 obj) {
			return kl::vec3(x - obj.x, y - obj.y, z - obj.z);
		}
		kl::vec3 operator * (float a) {
			return kl::vec3(x * a, y * a, z * a);
		}
		kl::vec3 operator / (float a) {
			return kl::vec3(x / a, y / a, z / a);
		}
		bool operator == (kl::vec3 obj) {
			return (x == obj.x && y == obj.y && z == obj.z);
		}
		bool operator != (kl::vec3 obj) {
			return (x != obj.x || y != obj.y || z != obj.z);
		}

		// Returns the vectors length
		float length() {
			return sqrt(x * x + y * y + z * z);
		}

		// Retruns a normalized vector
		kl::vec3 normalize() {
			float vecLen = 1 / length();
			return kl::vec3(x * vecLen, y * vecLen, z * vecLen);
		}

		// Returns the dot product
		float dot(kl::vec3 a) {
			return x * a.x + y * a.y + z * a.z;
		}

		// Returns the dot product
		kl::vec3 cross(kl::vec3 a) {
			return kl::vec3(y * a.z - z * a.y, z * a.x - x * a.z, x * a.y - y * a.x);
		}

		// Prints the data to the console
		void print() {
			printf("%lf %lf %lf\n", x, y, z);
		}
	}; 
}
