#pragma once

#include <iostream>


namespace kl {
	class ivec2 {
	public:
		int x;
		int y;

		// Constructors
		ivec2();
		ivec2(int x, int y);

		// Addition
		kl::ivec2 add(const kl::ivec2& obj) const;
		kl::ivec2 operator+(const kl::ivec2& obj) const;
		void operator+=(const kl::ivec2& obj);

		// Subtraction
		kl::ivec2 sub(const kl::ivec2& obj) const;
		kl::ivec2 operator-(const kl::ivec2& obj) const;
		void operator-=(const kl::ivec2& obj);
		
		// Multiplication
		kl::ivec2 mul(int a) const;
		kl::ivec2 operator*(int a) const;
		void operator*=(int a);
		kl::ivec2 mul(const kl::ivec2& obj) const;
		kl::ivec2 operator*(const kl::ivec2& obj) const;
		void operator*=(const kl::ivec2& obj);
		
		// Division
		kl::ivec2 div(int a) const;
		kl::ivec2 operator/(int a) const;
		void operator/=(int a);
		kl::ivec2 div(const kl::ivec2& obj) const;
		kl::ivec2 operator/(const kl::ivec2& obj) const;
		void operator/=(const kl::ivec2& obj);
		
		// Comparison
		bool equals(const kl::ivec2& obj) const;
		bool operator==(const kl::ivec2& obj) const;
		bool operator!=(const kl::ivec2& obj) const;

		// Returns a negated vector
		kl::ivec2 negate() const;

		// Returns an absolute vector
		kl::ivec2 abso() const;
	};

	// Overloading std::cout
	std::ostream& operator<<(std::ostream& os, const kl::ivec2& obj);
}
