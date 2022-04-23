#pragma once

#include <iostream>

#include "math/float2.h"
#include "math/float3.h"
#include "math/int4.h"


namespace kl {
	class float4 {
	public:
		union {
			struct {
				float x, y, z, w;
			};
			struct {
				float r, g, b, a;
			};
		};

		// Constr
		float4();
		float4(float a);
		float4(float x, float y, float z, float w);
		float4(const kl::int4& v);
		float4(const kl::float2& v, float z, float w);
		float4(float x, const kl::float2& v, float w);
		float4(float x, float y, const kl::float2& v);
		float4(const kl::float2& v1, const kl::float2& v2);
		float4(const kl::float3& v);
		float4(const kl::float3& v, float w);
		float4(float x, const kl::float3& v);
		float4(const kl::color& c);

		// Getter
		kl::float3 xyz() const;

		// Addition
		kl::float4 add(const kl::float4& obj) const;
		kl::float4 operator+(const kl::float4& obj) const;
		void operator+=(const kl::float4& obj);

		// Subtraction
		kl::float4 sub(const kl::float4& obj) const;
		kl::float4 operator-(const kl::float4& obj) const;
		void operator-=(const kl::float4& obj);

		// Multiplication
		kl::float4 mul(float a) const;
		kl::float4 operator*(float a) const;
		void operator*=(float a);
		kl::float4 mul(const kl::float4& obj) const;
		kl::float4 operator*(const kl::float4& obj) const;
		void operator*=(const kl::float4& obj);

		// Division
		kl::float4 div(float a) const;
		kl::float4 operator/(float a) const;
		void operator/=(float a);
		kl::float4 div(const kl::float4& obj) const;
		kl::float4 operator/(const kl::float4& obj) const;
		void operator/=(const kl::float4& obj);

		// Comparison
		bool equals(const kl::float4& obj) const;
		bool operator==(const kl::float4& obj) const;
		bool operator!=(const kl::float4& obj) const;

		// Returns a negated vector
		kl::float4 negate() const;

		// Returns an absolute vector
		kl::float4 abso() const;

		// Returns the vectors length
		float length() const;

		// Retruns a normalized vector
		kl::float4 normalize() const;

		// Returns the dot product
		float dot(const kl::float4& a) const;

		// Returns the angle between the given vector and self
		float angle(const kl::float4& a) const;
	};

	// Overloading std::cout
	std::ostream& operator<<(std::ostream& os, const kl::float4& obj);
}
