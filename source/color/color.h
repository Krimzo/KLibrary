#pragma once 

#include <iostream>
#include <windows.h>


namespace kl
{
	class color
	{
	public:
		// Had to flip R and B because the winapi uses BGR
		byte b;
		byte g;
		byte r;
		byte a;

		// Constructors
		color();
		color(byte r, byte g, byte b, byte a = 255);

		// Operator overloading
		bool equals(const kl::color& obj) const;
		bool operator==(const kl::color& obj) const;
		bool operator!=(const kl::color& obj) const;

		// Returns data as an int
		int getInt() const;

		// Returns grayscaled color
		kl::color grayscale() const;

		// Mixes 2 colors
		kl::color mix(const kl::color& col, float ratio) const;
		kl::color mix(const kl::color& col) const;
	};

	// Overloading std::cout
	std::ostream& operator<<(std::ostream& os, const kl::color& obj);

	// Predefined colors
	namespace colors
	{
		static const kl::color defaul = kl::color(204, 204, 204);
		static const kl::color black = kl::color(0, 0, 0);
		static const kl::color white = kl::color(255, 255, 255);
		static const kl::color gray = kl::color(50, 50, 50);
		static const kl::color lgray = kl::color(100, 100, 100);
		static const kl::color red = kl::color(255, 0, 0);
		static const kl::color green = kl::color(0, 255, 0);
		static const kl::color blue = kl::color(0, 0, 255);
		static const kl::color cyan = kl::color(30, 180, 170);
		static const kl::color purple = kl::color(220, 0, 220);
		static const kl::color yellow = kl::color(220, 220, 0);
		static const kl::color orange = kl::color(255, 140, 0);
		static const kl::color magenta = kl::color(155, 0, 155);
		static const kl::color crimson = kl::color(100, 0, 0);
		static const kl::color wheat = kl::color(245, 220, 180);
		static const kl::color sky = kl::color(190, 245, 255);
	};
}
