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
		vec3(kl::vec2 v, float z) {
			x = v.x;
			y = v.y;
			this->z = z;
		}
		vec3(float x, kl::vec2 v) {
			this->x = x;
			y = v.x;
			z = v.y;
		}
		vec3(kl::color color) {
			static const float toFloatColor = 1.0f / 255;
			x = color.r * toFloatColor;
			y = color.g * toFloatColor;
			z = color.b * toFloatColor;
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
			return operator*(1 / a);
		}
		bool operator == (kl::vec3 obj) {
			return (x == obj.x && y == obj.y && z == obj.z);
		}
		bool operator != (kl::vec3 obj) {
			return !operator==(obj);
		}

		// Returns a negated vector
		kl::vec3 negate() {
			return operator*(-1);
		}

		// Returns the vectors length
		float length() {
			return sqrt(x * x + y * y + z * z);
		}

		// Retruns a normalized vector
		kl::vec3 normalize() {
			return operator/(length());
		}

		// Returns the dot product
		float dot(kl::vec3 a) {
			return x * a.x + y * a.y + z * a.z;
		}

		// Returns the dot product
		kl::vec3 cross(kl::vec3 a) {
			return kl::vec3(y * a.z - z * a.y, z * a.x - x * a.z, x * a.y - y * a.x);
		}

		// Returns the angle between the given vector and self
		float angle(kl::vec3 a) {
			return acos(normalize().dot(a.normalize()));
		}

		// Returns the rotated vector around custom axis
		kl::vec3 rotate(float angle, kl::vec3 axis) {
			// Calculating trig funcs
			static const float halfeAngleRadians = 0.00872664625f;
			const float angleSin = sin(angle * halfeAngleRadians);
			const float angleCos = cos(angle * halfeAngleRadians);
			
			// Calculating quaternion consts
			const float qx = axis.x * angleSin;
			const float qy = axis.y * angleSin;
			const float qz = axis.z * angleSin;
			const float x2 = qx * qx;
			const float y2 = qy * qy;
			const float z2 = qz * qz;
			const float w2 = angleCos * angleCos;
			const float xy = qx * qy;
			const float xz = qx * qz;
			const float yz = qy * qz;
			const float xw = qx * angleCos;
			const float yw = qy * angleCos;
			const float zw = qz * angleCos;

			// Calculating the rotated vector
			kl::vec3 temp;
			temp.x = (w2 + x2 - z2 - y2) * x + (-zw + xy - zw + xy) * y + (yw + xz + xz + yw) * z;
			temp.y = (xy + zw + zw + xy) * x + ( y2 - z2 + w2 - x2) * y + (yz + yz - xw - xw) * z;
			temp.z = (xz - yw + xz - yw) * x + ( yz + yz + xw + xw) * y + (z2 - y2 - x2 + w2) * z;
			return temp;
		}

		// Prints the data to the console
		void print() {
			printf("%.2f %.2f %.2f\n", x, y, z);
		}
	}; 
}
