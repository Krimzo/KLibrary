#pragma once 

#include <iostream>

#include "KLTypes.h"


namespace kl {
	class Color {
	public:
		byte b, g, r, a;

		Color();

		Color(byte r, byte g, byte b, byte a = 255);

		bool operator==(const Color& obj) const;
		bool operator!=(const Color& obj) const;

		Color asGray() const;

		Color asInverted() const;

		char asASCII() const;

		Color mix(const Color& color, float ratio) const;
		Color mix(const Color& color) const;
	};

	// std::cout
	std::ostream& operator<<(std::ostream& stream, const Color& color);

	// Predefined
	namespace Colors {
		inline const Color Default = Color(204, 204, 204);
		inline const Color Black = Color(0, 0, 0);
		inline const Color White = Color(255, 255, 255);
		inline const Color Gray = Color(50, 50, 50);
		inline const Color Red = Color(255, 0, 0);
		inline const Color Green = Color(0, 255, 0);
		inline const Color Blue = Color(0, 0, 255);
		inline const Color Cyan = Color(30, 180, 170);
		inline const Color Purple = Color(220, 0, 220);
		inline const Color Yellow = Color(220, 220, 0);
		inline const Color Orange = Color(255, 140, 0);
		inline const Color Magenta = Color(155, 0, 155);
		inline const Color Crimson = Color(100, 0, 0);
		inline const Color Wheat = Color(245, 220, 180);
		inline const Color Sky = Color(190, 245, 255);
	};
}
