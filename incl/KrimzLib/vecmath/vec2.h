#pragma once

#include <iostream>

#include "KrimzLib/vecmath/ivec2.h"


namespace kl {
    class vec2 {
	public:
		float x;
		float y;

		// Constructors
		vec2();
		vec2(float a);
		vec2(float x, float y);
		vec2(const kl::ivec2& p);
		vec2(const kl::vec2& a, const kl::vec2& b);

		// Addition
		kl::vec2 add(const kl::vec2& obj) const;
		kl::vec2 operator+(const kl::vec2& obj) const;
		void operator+=(const kl::vec2& obj);

		// Subtraction
		kl::vec2 sub(const kl::vec2& obj) const;
		kl::vec2 operator-(const kl::vec2& obj) const;
		void operator-=(const kl::vec2& obj);

		// Multiplication
		kl::vec2 mul(float a) const;
		kl::vec2 operator*(float a) const;
		void operator*=(float a);
		kl::vec2 mul(const kl::vec2& obj) const;
		kl::vec2 operator*(const kl::vec2& obj) const;
		void operator*=(const kl::vec2& obj);

		// Division
		kl::vec2 div(float a) const;
		kl::vec2 operator/(float a) const;
		void operator/=(float a);
		kl::vec2 div(const kl::vec2& obj) const;
		kl::vec2 operator/(const kl::vec2& obj) const;
		void operator/=(const kl::vec2& obj);

		// Comparison
		bool equals(const kl::vec2& obj) const;
		bool operator==(const kl::vec2& obj) const;
		bool operator!=(const kl::vec2& obj) const;

		// Returns a negated vector
		kl::vec2 negate() const;

		// Returns an absolute vector
		kl::vec2 abso() const;

		// Returns the vectors length
		float length() const;

		// Retruns a normalized vector
		kl::vec2 normalize() const;

		// Returns the dot product
		float dot(const kl::vec2& a) const;

		// Returns the angle between the given vector and self
		float angle(const kl::vec2& a, bool full = false) const;

		// Returns a rotated vector around the given point
		kl::vec2 rotate(float angle) const;

		// Constans
		static const kl::vec2 pos_x;
		static const kl::vec2 neg_x;
		static const kl::vec2 pos_y;
		static const kl::vec2 neg_y;
	};

	// Overloading std::cout
	std::ostream& operator<<(std::ostream& os, const kl::vec2& obj);
}
