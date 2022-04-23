#pragma once

#include <iostream>

#include "math/int2.h"


namespace kl {
	class int3 {
	public:
		int x, y, z;

		// Constr
		int3();
		int3(int a);
		int3(int x, int y, int z);
		int3(const kl::int2& v, int z);
		int3(int x, const kl::int2& v);

		// Addition
		kl::int3 add(const kl::int3& obj) const;
		kl::int3 operator+(const kl::int3& obj) const;
		void operator+=(const kl::int3& obj);

		// Subtraction
		kl::int3 sub(const kl::int3& obj) const;
		kl::int3 operator-(const kl::int3& obj) const;
		void operator-=(const kl::int3& obj);

		// Multiplication
		kl::int3 mul(int a) const;
		kl::int3 operator*(int a) const;
		void operator*=(int a);
		kl::int3 mul(const kl::int3& obj) const;
		kl::int3 operator*(const kl::int3& obj) const;
		void operator*=(const kl::int3& obj);

		// Division
		kl::int3 div(int a) const;
		kl::int3 operator/(int a) const;
		void operator/=(int a);
		kl::int3 div(const kl::int3& obj) const;
		kl::int3 operator/(const kl::int3& obj) const;
		void operator/=(const kl::int3& obj);

		// Comparison
		bool equals(const kl::int3& obj) const;
		bool operator==(const kl::int3& obj) const;
		bool operator!=(const kl::int3& obj) const;

		// Returns a negated vector
		kl::int3 negate() const;

		// Returns an absolute vector
		kl::int3 abso() const;
	};

	// Overloading std::cout
	std::ostream& operator<<(std::ostream& os, const kl::int3& obj);
}
