#pragma once

#include <iostream>

#include "KrimzLib/vecmath/vec2.h"
#include "KrimzLib/graphics/color.h"


namespace kl {
    class vec3 {
	public:
		float x;
		float y;
		float z;

		// Constructors
		vec3();
		vec3(float a);
		vec3(float x, float y, float z);
		vec3(const kl::vec2& v, float z);
		vec3(float x, const kl::vec2& v);

		// Addition
		kl::vec3 add(const kl::vec3& obj) const;
		kl::vec3 operator+(const kl::vec3& obj) const;
		void operator+=(const kl::vec3& obj);

		// Subtraction
		kl::vec3 sub(const kl::vec3& obj) const;
		kl::vec3 operator-(const kl::vec3& obj) const;
		void operator-=(const kl::vec3& obj);

		// Multiplication
		kl::vec3 mul(float a) const;
		kl::vec3 operator*(float a) const;
		void operator*=(float a);
		kl::vec3 mul(const kl::vec3& obj) const;
		kl::vec3 operator*(const kl::vec3& obj) const;
		void operator*=(const kl::vec3& obj);

		// Division
		kl::vec3 div(float a) const;
		kl::vec3 operator/(float a) const;
		void operator/=(float a);
		kl::vec3 div(const kl::vec3& obj) const;
		kl::vec3 operator/(const kl::vec3& obj) const;
		void operator/=(const kl::vec3& obj);

		// Comparison
		bool equals(const kl::vec3& obj) const;
		bool operator==(const kl::vec3& obj) const;
		bool operator!=(const kl::vec3& obj) const;

		// Returns a negated vector
		kl::vec3 negate() const;

		// Returns an absolute vector
		kl::vec3 abso() const;

		// Returns the vectors length
		float length() const;

		// Retruns a normalized vector
		kl::vec3 normalize() const;

		// Returns the dot product
		float dot(const kl::vec3& a) const;

		// Returns the dot product
		kl::vec3 cross(const kl::vec3& a) const;

		// Returns the angle between the given vector and self
		float angle(const kl::vec3& a) const;

		// Returns a rotated vector around the given axis
		kl::vec3 rotate(float angle, const kl::vec3& axis) const;
	};

	// Overloading std::cout
	std::ostream& operator<<(std::ostream& os, const kl::vec3& obj);
}
