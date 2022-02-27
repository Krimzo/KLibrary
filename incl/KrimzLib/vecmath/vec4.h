#pragma once

#include <iostream>

#include "KrimzLib/vecmath/vec2.h"
#include "KrimzLib/vecmath/vec3.h"


namespace kl {
	class vec4 {
	public:
		float x;
		float y;
		float z;
		float w;

		// Constructors
		vec4();
		vec4(float a);
		vec4(float x, float y, float z, float w);
		vec4(const kl::vec2& v, float z, float w);
		vec4(float x, const kl::vec2& v, float w);
		vec4(float x, float y, const kl::vec2& v);
		vec4(const kl::vec2& v1, const kl::vec2& v2);
		vec4(const kl::vec3& v);
		vec4(const kl::vec3& v, float w);
		vec4(float x, const kl::vec3& v);
		vec4(const kl::color& color);

		// Getter
		kl::vec3 xyz() const;

		// Addition
		kl::vec4 add(const kl::vec4& obj) const;
		kl::vec4 operator+(const kl::vec4& obj) const;
		void operator+=(const kl::vec4& obj);

		// Subtraction
		kl::vec4 sub(const kl::vec4& obj) const;
		kl::vec4 operator-(const kl::vec4& obj) const;
		void operator-=(const kl::vec4& obj);

		// Multiplication
		kl::vec4 mul(float a) const;
		kl::vec4 operator*(float a) const;
		void operator*=(float a);
		kl::vec4 mul(const kl::vec4& obj) const;
		kl::vec4 operator*(const kl::vec4& obj) const;
		void operator*=(const kl::vec4& obj);

		// Division
		kl::vec4 div(float a) const;
		kl::vec4 operator/(float a) const;
		void operator/=(float a);
		kl::vec4 div(const kl::vec4& obj) const;
		kl::vec4 operator/(const kl::vec4& obj) const;
		void operator/=(const kl::vec4& obj);

		// Comparison
		bool equals(const kl::vec4& obj) const;
		bool operator==(const kl::vec4& obj) const;
		bool operator!=(const kl::vec4& obj) const;

		// Returns a negated vector
		kl::vec4 negate() const;

		// Returns an absolute vector
		kl::vec4 abso() const;

		// Returns the vectors length
		float length() const;

		// Retruns a normalized vector
		kl::vec4 normalize() const;

		// Returns the dot product
		float dot(const kl::vec4& a) const;

		// Returns the angle between the given vector and self
		float angle(const kl::vec4& a) const;
	};

	// Overloading std::cout
	std::ostream& operator<<(std::ostream& os, const kl::vec4& obj);
}
