#pragma once


namespace kl {
	struct mat3 {
		// Constructor
		mat3() {
			setIdentity();
		}

		// Operator overloading
		float& operator [] (int i) {
			return data[i];
		}
		kl::mat3 operator + (kl::mat3& obj) {
			kl::mat3 temp;
			for (int i = 0; i < 9; i++) {
				temp[i] = data[i] + obj[i];
			}
			return temp;
		}
		kl::mat3 operator + (kl::mat3&& obj) {
			operator+(obj);
		}
		void operator += (kl::mat3& obj) {
			for (int i = 0; i < 9; i++) {
				data[i] += obj[i];
			}
		}
		void operator += (kl::mat3&& obj) {
			operator+=(obj);
		}
		kl::mat3 operator - (kl::mat3& obj) {
			kl::mat3 temp;
			for (int i = 0; i < 9; i++) {
				temp[i] = data[i] - obj[i];
			}
			return temp;
		}
		kl::mat3 operator - (kl::mat3&& obj) {
			operator-(obj);
		}
		void operator -= (kl::mat3& obj) {
			for (int i = 0; i < 9; i++) {
				data[i] -= obj[i];
			}
		}
		void operator -= (kl::mat3&& obj) {
			operator-=(obj);
		}
		kl::mat3 operator * (float a) {
			kl::mat3 temp;
			for (int i = 0; i < 9; i++) {
				temp[i] = data[i] * a;
			}
			return temp;
		}
		void operator *= (float a) {
			for (int i = 0; i < 9; i++) {
				data[i] *= a;
			}
		}
		kl::vec3 operator * (kl::vec3 obj) {
			kl::vec3 temp;
			temp.x = data[0] * obj.x + data[1] * obj.y + data[2] * obj.z;
			temp.y = data[3] * obj.x + data[4] * obj.y + data[5] * obj.z;
			temp.z = data[6] * obj.x + data[7] * obj.y + data[8] * obj.z;
			return temp;
		}
		kl::mat3 operator * (kl::mat3& obj) {
			kl::mat3 temp;
			temp[0] = data[0] * obj[0] + data[1] * obj[3] + data[2] * obj[6];
			temp[1] = data[0] * obj[1] + data[1] * obj[4] + data[2] * obj[7];
			temp[2] = data[0] * obj[2] + data[1] * obj[5] + data[2] * obj[8];
			temp[3] = data[3] * obj[0] + data[4] * obj[3] + data[5] * obj[6];
			temp[4] = data[3] * obj[1] + data[4] * obj[4] + data[5] * obj[7];
			temp[5] = data[3] * obj[2] + data[4] * obj[5] + data[5] * obj[8];
			temp[6] = data[6] * obj[0] + data[7] * obj[3] + data[8] * obj[6];
			temp[7] = data[6] * obj[1] + data[7] * obj[4] + data[8] * obj[7];
			temp[8] = data[6] * obj[2] + data[7] * obj[5] + data[8] * obj[8];
			return temp;
		}
		kl::mat3 operator * (kl::mat3&& obj) {
			return operator*(obj);
		}
		void operator *= (kl::mat3& obj) {
			kl::mat3 temp = operator*(obj);
			for (int i = 0; i < 9; i++) {
				data[i] = temp[i];
			}
		}
		void operator *= (kl::mat3&& obj) {
			operator*=(obj);
		}
		kl::mat3 operator / (float a) {
			return operator*(1 / a);
		}
		void operator /= (float a) {
			operator*=(1 / a);
		}
		bool operator == (kl::mat3& obj) {
			for (int i = 0; i < 9; i++) {
				if (data[i] != obj[i]) {
					return false;
				}
			}
			return true;
		}
		bool operator == (kl::mat3&& obj) {
			operator==(obj);
		}
		bool operator != (kl::mat3& obj) {
			return !operator==(obj);
		}
		bool operator != (kl::mat3&& obj) {
			operator!=(obj);
		}

		// Returns pointer to raw data
		float* pointer() {
			return data;
		}

		// Loads the identity matrix
		void setIdentity() {
			data[0] = 1; data[1] = 0; data[2] = 0;
			data[3] = 0; data[4] = 1; data[5] = 0;
			data[6] = 0; data[7] = 0; data[8] = 1;
		}

		// Prints the matrix to the screen
		void print() {
			printf(
				"% .2f % .2f % .2f\n"
				"% .2f % .2f % .2f\n"
				"% .2f % .2f % .2f\n",
				data[0], data[1], data[2],
				data[3], data[4], data[5],
				data[6], data[7], data[8]
			);
		}

		// Returns a translation matrix
		static kl::mat3 translate(kl::vec2 translation) {
			kl::mat3 temp;
			temp[2] = translation.x;
			temp[5] = translation.y;
			return temp;
		}

		// Returns a rotation matrix
		static kl::mat3 rotate(float rotation) {
			// Computing trig
			const float zSin = sin(rotation * 0.01745329251f);
			const float zCos = cos(rotation * 0.01745329251f);

			// Generating the mat
			kl::mat3 temp;
			temp[0] = zCos;
			temp[1] = -zSin;
			temp[3] = zSin;
			temp[4] = zCos;
			return temp;
		}

		// Returns a scaling matrix
		static kl::mat3 scale(kl::vec2 size) {
			kl::mat3 temp;
			temp[0] = size.x;
			temp[4] = size.y;
			return temp;
		}

	private:
		// Variables
		float data[9];
	};
}
