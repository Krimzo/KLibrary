#pragma once

#include <iostream>

#include "math/float2.h"
#include "math/int3.h"
#include "color/color.h"


namespace kl {
	class float3 {
	public:
		union {
			struct {
				float x, y, z;
			};
			struct {
				float r, g, b;
			};
		};

		// Constr
		float3();
		float3(float a);
		float3(float x, float y, float z);
		float3(const kl::int3& v);
		float3(const kl::float2& v, float z);
		float3(float x, const kl::float2& v);
		float3(const kl::color& c);

		// Getter
		kl::float2 xy() const;

		// Addition
		kl::float3 add(const kl::float3& obj) const;
		kl::float3 operator+(const kl::float3& obj) const;
		void operator+=(const kl::float3& obj);

		// Subtraction
		kl::float3 sub(const kl::float3& obj) const;
		kl::float3 operator-(const kl::float3& obj) const;
		void operator-=(const kl::float3& obj);

		// Multiplication
		kl::float3 mul(float a) const;
		kl::float3 operator*(float a) const;
		void operator*=(float a);
		kl::float3 mul(const kl::float3& obj) const;
		kl::float3 operator*(const kl::float3& obj) const;
		void operator*=(const kl::float3& obj);

		// Division
		kl::float3 div(float a) const;
		kl::float3 operator/(float a) const;
		void operator/=(float a);
		kl::float3 div(const kl::float3& obj) const;
		kl::float3 operator/(const kl::float3& obj) const;
		void operator/=(const kl::float3& obj);

		// Comparison
		bool equals(const kl::float3& obj) const;
		bool operator==(const kl::float3& obj) const;
		bool operator!=(const kl::float3& obj) const;

		// Returns a negated vector
		kl::float3 negate() const;

		// Returns an absolute vector
		kl::float3 abso() const;

		// Returns the vectors length
		float length() const;

		// Retruns a normalized vector
		kl::float3 normalize() const;

		// Returns the dot product
		float dot(const kl::float3& a) const;

		// Returns the dot product
		kl::float3 cross(const kl::float3& a) const;

		// Returns the angle between the given vector and self
		float angle(const kl::float3& a) const;

		// Returns a rotated vector around the given axis
		kl::float3 rotate(float angle, const kl::float3& axis) const;

		// Constans
		static const kl::float3 pos_x;
		static const kl::float3 neg_x;
		static const kl::float3 pos_y;
		static const kl::float3 neg_y;
		static const kl::float3 pos_z;
		static const kl::float3 neg_z;
	};

	// Overloading std::cout
	std::ostream& operator<<(std::ostream& os, const kl::float3& obj);
}
