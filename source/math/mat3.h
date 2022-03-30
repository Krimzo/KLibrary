#pragma once

#include <iostream>

#include "math/float2.h"
#include "math/float3.h"


namespace kl {
	class mat3 {
	private:
		float data[9] = {};

	public:
		// Constructor
		mat3();

		// Getter
		float& operator[](int i);
		
		// Addition
		kl::mat3 add(const kl::mat3& obj) const;
		kl::mat3 operator+(const kl::mat3& obj) const;
		void operator+=(const kl::mat3& obj);

		// Subtraction
		kl::mat3 sub(const kl::mat3& obj) const;
		kl::mat3 operator-(const kl::mat3& obj) const;
		void operator-=(const kl::mat3& obj);

		// Multiplication
		kl::mat3 mul(float a) const;
		kl::mat3 operator*(float a) const;
		void operator*=(float a);
		kl::mat3 mul(const kl::mat3& obj) const;
		kl::mat3 operator*(const kl::mat3& obj) const;
		void operator*=(const kl::mat3& obj);
		kl::float3 mul(const kl::float3& obj) const;
		kl::float3 operator*(const kl::float3& obj) const;

		// Division
		kl::mat3 div(float a) const;
		kl::mat3 operator/(float a) const;
		void operator/=(float a);

		// Comparison
		bool equals(const kl::mat3& obj) const;
		bool operator==(const kl::mat3& obj) const;
		bool operator!=(const kl::mat3& obj) const;

		// Returns pointer to raw data
		float* pointer() const;

		// Loads the identity matrix
		void setIdentity();

		// Returns the inverse matrix
		kl::mat3 inverse() const;

		// Returns a translation matrix
		static kl::mat3 translate(const kl::float2& translation);

		// Returns a rotation matrix
		static kl::mat3 rotate(float rotation);

		// Returns a scaling matrix
		static kl::mat3 scale(const kl::float2& size);
	};

	// Overloading std::cout
	std::ostream& operator<<(std::ostream& os, kl::mat3& obj);
	std::ostream& operator<<(std::ostream& os, kl::mat3&& obj);
}
