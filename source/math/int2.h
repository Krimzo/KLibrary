#pragma once

#include <iostream>


namespace kl
{
	class int2
	{
	public:
		int x;
		int y;

		// Constructors
		int2();
		int2(int a);
		int2(int x, int y);

		// Addition
		kl::int2 add(const kl::int2& obj) const;
		kl::int2 operator+(const kl::int2& obj) const;
		void operator+=(const kl::int2& obj);

		// Subtraction
		kl::int2 sub(const kl::int2& obj) const;
		kl::int2 operator-(const kl::int2& obj) const;
		void operator-=(const kl::int2& obj);

		// Multiplication
		kl::int2 mul(int a) const;
		kl::int2 operator*(int a) const;
		void operator*=(int a);
		kl::int2 mul(const kl::int2& obj) const;
		kl::int2 operator*(const kl::int2& obj) const;
		void operator*=(const kl::int2& obj);

		// Division
		kl::int2 div(int a) const;
		kl::int2 operator/(int a) const;
		void operator/=(int a);
		kl::int2 div(const kl::int2& obj) const;
		kl::int2 operator/(const kl::int2& obj) const;
		void operator/=(const kl::int2& obj);

		// Comparison
		bool equals(const kl::int2& obj) const;
		bool operator==(const kl::int2& obj) const;
		bool operator!=(const kl::int2& obj) const;

		// Returns a negated vector
		kl::int2 negate() const;

		// Returns an absolute vector
		kl::int2 abso() const;
	};

	// Overloading std::cout
	std::ostream& operator<<(std::ostream& os, const kl::int2& obj);
}
