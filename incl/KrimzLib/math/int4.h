#pragma once

#include <iostream>

#include "KrimzLib/math/int2.h"
#include "KrimzLib/math/int3.h"


namespace kl {
	class int4 {
	public:
		int x;
		int y;
		int z;
		int w;

		// Constructors
		int4();
		int4(int a);
		int4(int x, int y, int z, int w);
		int4(const kl::int2& v, int z, int w);
		int4(int x, const kl::int2& v, int w);
		int4(int x, int y, const kl::int2& v);
		int4(const kl::int2& v1, const kl::int2& v2);
		int4(const kl::int3& v, int w);
		int4(int x, const kl::int3& v);

		// Addition
		kl::int4 add(const kl::int4& obj) const;
		kl::int4 operator+(const kl::int4& obj) const;
		void operator+=(const kl::int4& obj);

		// Subtraction
		kl::int4 sub(const kl::int4& obj) const;
		kl::int4 operator-(const kl::int4& obj) const;
		void operator-=(const kl::int4& obj);

		// Multiplication
		kl::int4 mul(int a) const;
		kl::int4 operator*(int a) const;
		void operator*=(int a);
		kl::int4 mul(const kl::int4& obj) const;
		kl::int4 operator*(const kl::int4& obj) const;
		void operator*=(const kl::int4& obj);

		// Division
		kl::int4 div(int a) const;
		kl::int4 operator/(int a) const;
		void operator/=(int a);
		kl::int4 div(const kl::int4& obj) const;
		kl::int4 operator/(const kl::int4& obj) const;
		void operator/=(const kl::int4& obj);

		// Comparison
		bool equals(const kl::int4& obj) const;
		bool operator==(const kl::int4& obj) const;
		bool operator!=(const kl::int4& obj) const;

		// Returns a negated vector
		kl::int4 negate() const;

		// Returns an absolute vector
		kl::int4 abso() const;
	};

	// Overloading std::cout
	std::ostream& operator<<(std::ostream& os, const kl::int4& obj);
}
