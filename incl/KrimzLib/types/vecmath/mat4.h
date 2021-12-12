#pragma once


namespace kl {
	struct mat4 {
		// Constructor
		mat4() {
			loadIdentity();
		}
		mat4(kl::vec4 row0, kl::vec4 row1, kl::vec4 row2, kl::vec4 row3) {
			data[ 0] = row0.x; data[ 1] = row0.y; data[ 2] = row0.z; data[ 3] = row0.w;
			data[ 4] = row1.x; data[ 5] = row1.y; data[ 6] = row1.z; data[ 7] = row1.w;
			data[ 8] = row2.x; data[ 9] = row2.y; data[10] = row2.z; data[11] = row2.w;
			data[12] = row3.x; data[13] = row3.y; data[14] = row3.z; data[15] = row3.w;
		}

		// Operator overloading
		float& operator [] (int i) {
			return data[i];
		}
		kl::mat4 operator + (kl::mat4& obj) {
			kl::mat4 temp;
			for (int i = 0; i < 16; i++) {
				temp[i] = data[i] + obj[i];
			}
			return temp;
		}
		kl::mat4 operator + (kl::mat4&& obj) {
			operator+(obj);
		}
		kl::mat4 operator - (kl::mat4& obj) {
			kl::mat4 temp;
			for (int i = 0; i < 16; i++) {
				temp[i] = data[i] - obj[i];
			}
			return temp;
		}
		kl::mat4 operator - (kl::mat4&& obj) {
			operator-(obj);
		}
		kl::mat4 operator * (float a) {
			kl::mat4 temp;
			for (int i = 0; i < 16; i++) {
				temp[i] = data[i] * a;
			}
			return temp;
		}
		kl::vec4 operator * (kl::vec4 obj) {
			kl::vec4 temp;
			temp.x = data[ 0] * obj.x + data[ 1] * obj.y + data[ 2] * obj.z + data[ 3] * obj.w;
			temp.y = data[ 4] * obj.x + data[ 5] * obj.y + data[ 6] * obj.z + data[ 7] * obj.w;
			temp.z = data[ 8] * obj.x + data[ 9] * obj.y + data[10] * obj.z + data[11] * obj.w;
			temp.w = data[12] * obj.x + data[13] * obj.y + data[14] * obj.z + data[15] * obj.w;
			return temp;
		}
		kl::mat4 operator * (kl::mat4& obj) {
			kl::mat4 temp;
			temp[ 0] = data[ 0] * obj[ 0] + data[ 1] * obj[ 4] + data[ 2] * obj[ 8] + data[ 3] * obj[12];
			temp[ 1] = data[ 0] * obj[ 1] + data[ 1] * obj[ 5] + data[ 2] * obj[ 9] + data[ 3] * obj[13];
			temp[ 2] = data[ 0] * obj[ 2] + data[ 1] * obj[ 6] + data[ 2] * obj[10] + data[ 3] * obj[14];
			temp[ 3] = data[ 0] * obj[ 3] + data[ 1] * obj[ 7] + data[ 2] * obj[11] + data[ 3] * obj[15];								 
			temp[ 4] = data[ 4] * obj[ 0] + data[ 5] * obj[ 4] + data[ 6] * obj[ 8] + data[ 7] * obj[12];
			temp[ 5] = data[ 4] * obj[ 1] + data[ 5] * obj[ 5] + data[ 6] * obj[ 9] + data[ 7] * obj[13];
			temp[ 6] = data[ 4] * obj[ 2] + data[ 5] * obj[ 6] + data[ 6] * obj[10] + data[ 7] * obj[14];
			temp[ 7] = data[ 4] * obj[ 3] + data[ 5] * obj[ 7] + data[ 6] * obj[11] + data[ 7] * obj[15];								 
			temp[ 8] = data[ 8] * obj[ 0] + data[ 9] * obj[ 4] + data[10] * obj[ 8] + data[11] * obj[12];
			temp[ 9] = data[ 8] * obj[ 1] + data[ 9] * obj[ 5] + data[10] * obj[ 9] + data[11] * obj[13];
			temp[10] = data[ 8] * obj[ 2] + data[ 9] * obj[ 6] + data[10] * obj[10] + data[11] * obj[14];
			temp[11] = data[ 8] * obj[ 3] + data[ 9] * obj[ 7] + data[10] * obj[11] + data[11] * obj[15];
			temp[12] = data[12] * obj[ 0] + data[13] * obj[ 4] + data[14] * obj[ 8] + data[15] * obj[12];
			temp[13] = data[12] * obj[ 1] + data[13] * obj[ 5] + data[14] * obj[ 9] + data[15] * obj[13];
			temp[14] = data[12] * obj[ 2] + data[13] * obj[ 6] + data[14] * obj[10] + data[15] * obj[14];
			temp[15] = data[12] * obj[ 3] + data[13] * obj[ 7] + data[14] * obj[11] + data[15] * obj[15];
			return temp;
		}
		kl::mat4 operator * (kl::mat4&& obj) {
			return operator*(obj);
		}
		kl::mat4 operator / (float a) {
			kl::mat4 temp;
			const float aInv = 1 / a;
			for (int i = 0; i < 16; i++) {
				temp[i] = data[i] * aInv;
			}
			return temp;
		}
		bool operator == (kl::mat4& obj) {
			for (int i = 0; i < 16; i++) {
				if (data[i] != obj[i]) {
					return false;
				}
			}
			return true;
		}
		bool operator == (kl::mat4&& obj) {
			operator==(obj);
		}
		bool operator != (kl::mat4& obj) {
			return !operator==(obj);
		}
		bool operator != (kl::mat4&& obj) {
			operator!=(obj);
		}

		// Returns pointer to raw data
		float* pointer() {
			return data;
		}

		// Loads the identity matrix
		void loadIdentity() {
			data[ 0] = 1; data[ 1] = 0; data[ 2] = 0; data[ 3] = 0;
			data[ 4] = 0; data[ 5] = 1; data[ 6] = 0; data[ 7] = 0;
			data[ 8] = 0; data[ 9] = 0; data[10] = 1; data[11] = 0;
			data[12] = 0; data[13] = 0; data[14] = 0; data[15] = 1;
		}

		// Prints the matrix to the screen
		void print() {
			printf(
				"%.2f %.2f %.2f %.2f\n%.2f %.2f %.2f %.2f\n%.2f %.2f %.2f %.2f\n%.2f %.2f %.2f %.2f\n",
				data[ 0], data[ 1], data[ 2], data[ 3],
				data[ 4], data[ 5], data[ 6], data[ 7],
				data[ 8], data[ 9], data[10], data[11],
				data[12], data[13], data[14], data[15]
			);
		}

		// Returns a translation matrix
		static kl::mat4 translate(kl::vec3 translation) {
			kl::mat4 temp;
			temp[ 3] = translation.x;
			temp[ 7] = translation.y;
			temp[11] = translation.z;
			return temp;
		}

		// Returns a rotation matrix
		static kl::mat4 rotate(kl::vec3 rotation) {
			// Computing x rotation matrix
			float xSin = sin(rotation.x * 0.01745329251f);
			float xCos = cos(rotation.x * 0.01745329251f);
			kl::mat4 xRot;
			xRot[ 5] =  xCos;
			xRot[ 6] = -xSin;
			xRot[ 9] =  xSin;
			xRot[10] =  xCos;

			// Computing y rotation matrix
			float ySin = sin(rotation.y * 0.01745329251f);
			float yCos = cos(rotation.y * 0.01745329251f);
			kl::mat4 yRot;
			yRot[ 0] =  yCos;
			yRot[ 2] =  ySin;
			yRot[ 8] = -ySin;
			yRot[10] =  yCos;

			// Computing z rotation matrix
			float zSin = sin(rotation.z * 0.01745329251f);
			float zCos = cos(rotation.z * 0.01745329251f);
			kl::mat4 zRot;
			zRot[0] =  zCos;
			zRot[1] = -zSin;
			zRot[4] =  zSin;
			zRot[5] =  zCos;

			// Multiplying and returning
			return zRot * yRot * xRot;
		}

		// Returns a scaling matrix
		static kl::mat4 scale(kl::vec3 size) {
			kl::mat4 temp;
			temp[ 0] = size.x;
			temp[ 5] = size.y;
			temp[10] = size.z;
			return temp;
		}

	private:
		// Variables
		float data[16];
	};
}
