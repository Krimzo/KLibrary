#pragma once


namespace kl {
    struct vec3 {
		double x;
		double y;
		double z;

		// Constructors
		vec3() {
			x = 0;
			y = 0;
			z = 0;
		}
		vec3(double x, double y, double z) {
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
		kl::vec3 operator * (double a) {
			return kl::vec3(x * a, y * a, z * a);
		}
		kl::vec3 operator / (double a) {
			return kl::vec3(x / a, y / a, z / a);
		}
		bool operator == (kl::vec3 obj) {
			return (x == obj.x && y == obj.y && z == obj.z);
		}
		bool operator != (kl::vec3 obj) {
			return (x != obj.x || y != obj.y || z != obj.z);
		}

		// Returns the vectors length
		double length() {
			return sqrt(x * x + y * y + z * z);
		}

		// Retruns a normalized vector
		kl::vec3 normalize() {
			double vecLen = 1 / length();
			return kl::vec3(x * vecLen, y * vecLen, z * vecLen);
		}

		// Prints the data to the console
		void print() {
			printf("%lf %lf %lf\n", x, y, z);
		}
	}; 
}
