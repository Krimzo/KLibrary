#pragma once

#include <iostream>

#include "KrimzLib/math/float4.h"


namespace kl {
	class mat4 {
	private:
		float data[16] = {};

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
		kl::float4 mul(const kl::float4& obj) const;
		kl::float4 operator*(const kl::float4& obj) const;

		// Division
		kl::mat4 div(float a) const;
		kl::mat4 operator/(float a) const;
		void operator/=(float a);

		// Comparison
		bool equals(const kl::mat4& obj) const;
		bool operator==(const kl::mat4& obj) const;
		bool operator!=(const kl::mat4& obj) const;

		// Returns pointer to raw data
		float* pointer() const;

		// Loads the identity matrix
		void setIdentity();

		// Returns inverse matrix if possible
		kl::mat4 inverse() const;

		// Returns a translation matrix
		static kl::mat4 translate(const kl::float3& translation);

		// Returns a rotation matrix
		static kl::mat4 rotate(const kl::float3& rotation);

		// Returns a scaling matrix
		static kl::mat4 scale(const kl::float3& size);

		// Returns the perspective projection matrix
		static kl::mat4 persp(float fov, float ar, float zNear, float zFar);

		// Returns the orthographics projection matrix
		static kl::mat4 ortho(float left, float right, float bottom, float top, float nearZ, float farZ);

		// Returns the "look at" matrix
		static kl::mat4 lookAt(const kl::float3& position, const kl::float3& target, const kl::float3& up);
	};

	// Overloading std::cout
	std::ostream& operator<<(std::ostream& os, kl::mat4& obj);
	std::ostream& operator<<(std::ostream& os, kl::mat4&& obj);
}
