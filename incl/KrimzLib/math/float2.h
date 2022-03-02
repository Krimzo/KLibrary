#pragma once

#include <iostream>

#include "KrimzLib/math/int2.h"


namespace kl {
    class float2 {
	public:
		float x;
		float y;

		// Constructors
		float2();
		float2(float a);
		float2(float x, float y);
		float2(const kl::int2& v);
		float2(const kl::float2& a, const kl::float2& b);

		// Addition
		kl::float2 add(const kl::float2& obj) const;
		kl::float2 operator+(const kl::float2& obj) const;
		void operator+=(const kl::float2& obj);

		// Subtraction
		kl::float2 sub(const kl::float2& obj) const;
		kl::float2 operator-(const kl::float2& obj) const;
		void operator-=(const kl::float2& obj);

		// Multiplication
		kl::float2 mul(float a) const;
		kl::float2 operator*(float a) const;
		void operator*=(float a);
		kl::float2 mul(const kl::float2& obj) const;
		kl::float2 operator*(const kl::float2& obj) const;
		void operator*=(const kl::float2& obj);

		// Division
		kl::float2 div(float a) const;
		kl::float2 operator/(float a) const;
		void operator/=(float a);
		kl::float2 div(const kl::float2& obj) const;
		kl::float2 operator/(const kl::float2& obj) const;
		void operator/=(const kl::float2& obj);

		// Comparison
		bool equals(const kl::float2& obj) const;
		bool operator==(const kl::float2& obj) const;
		bool operator!=(const kl::float2& obj) const;

		// Returns a negated vector
		kl::float2 negate() const;

		// Returns an absolute vector
		kl::float2 abso() const;

		// Returns the vectors length
		float length() const;

		// Retruns a normalized vector
		kl::float2 normalize() const;

		// Returns the dot product
		float dot(const kl::float2& a) const;

		// Returns the angle between the given vector and self
		float angle(const kl::float2& a, bool full = false) const;

		// Returns a rotated vector around the given point
		kl::float2 rotate(float angle) const;

		// Constans
		static const kl::float2 pos_x;
		static const kl::float2 neg_x;
		static const kl::float2 pos_y;
		static const kl::float2 neg_y;
	};

	// Overloading std::cout
	std::ostream& operator<<(std::ostream& os, const kl::float2& obj);
}
