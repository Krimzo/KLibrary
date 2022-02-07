#pragma once

#include "KrimzLib/vecmath/vec4.h"


namespace kl {
	class mat4 {
	private:
		float data[16];

	public:
		// Constructor
		mat4();

		// Getter
		float& operator[](int i);

		// Addition
		kl::mat4 add(const kl::mat4& obj) const;
		kl::mat4 operator+(const kl::mat4& obj) const;
		void operator+=(const kl::mat4& obj);

		// Subtraction
		kl::mat4 sub(const kl::mat4& obj) const;
		kl::mat4 operator-(const kl::mat4& obj) const;
		void operator-=(const kl::mat4& obj);

		// Multiplication
		kl::mat4 mul(float a) const;
		kl::mat4 operator*(float a) const;
		void operator*=(float a);
		kl::mat4 mul(const kl::mat4& obj) const;
		kl::mat4 operator*(const kl::mat4& obj) const;
		void operator*=(const kl::mat4& obj);
		kl::vec4 mul(const kl::vec4& obj) const;
		kl::vec4 operator*(const kl::vec4& obj) const;

		// Division
		kl::mat4 div(float a) const;
		kl::mat4 operator/(float a) const;
		void operator/=(float a);

		// Comparison
		bool equals(const kl::mat4& obj) const;
		bool operator==(const kl::mat4& obj) const;
		bool operator!=(const kl::mat4& obj) const;

		// Returns pointer to raw data
		float* pointer();

		// Loads the identity matrix
		void setIdentity();

		// Prints the matrix to the screen
		void print() const;

		// Returns a translation matrix
		static kl::mat4 translate(const kl::vec3& translation);

		// Returns a rotation matrix
		static kl::mat4 rotate(const kl::vec3& rotation);

		// Returns a scaling matrix
		static kl::mat4 scale(const kl::vec3& size);

		// Returns the perspective projection matrix
		static kl::mat4 perspective(float fov, float ar, float zNear, float zFar);

		// Returns the orthographics projection matrix
		static kl::mat4 ortho(float left, float right, float bottom, float top, float nearZ, float farZ);

		// Returns the camera view matrix
		static kl::mat4 view(const kl::vec3& forward, const kl::vec3& right, const kl::vec3& up, const kl::vec3& position);

		// Returns the "look at" matrix
		static kl::mat4 lookAt(const kl::vec3& eye, const kl::vec3& center, const kl::vec3& up);
	};
}
