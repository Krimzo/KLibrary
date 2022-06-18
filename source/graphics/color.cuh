#pragma once 

#include <iostream>

#include "cuda/cu_types.cuh"


using byte = unsigned char;

namespace kl {
	class color {
	public:
		byte b, g, r, a;

		ALL color();
		ALL color(byte r, byte g, byte b, byte a = 255);

		ALL bool equ(const kl::color& obj) const;
		ALL bool operator==(const kl::color& obj) const;
		ALL bool operator!=(const kl::color& obj) const;

		ALL kl::color gray() const;
		ALL kl::color invert() const;
		ALL char ascii() const;

		ALL kl::color mix(const kl::color& col, float ratio) const;
		ALL kl::color mix(const kl::color& col) const;
	};

	// std::cout
	std::ostream& operator<<(std::ostream& os, const kl::color& obj);

	// Predefined
	namespace colors {
		inline const kl::color defaul = kl::color(204, 204, 204);
		inline const kl::color black = kl::color(0, 0, 0);
		inline const kl::color white = kl::color(255, 255, 255);
		inline const kl::color gray = kl::color(50, 50, 50);
		inline const kl::color lgray = kl::color(100, 100, 100);
		inline const kl::color red = kl::color(255, 0, 0);
		inline const kl::color green = kl::color(0, 255, 0);
		inline const kl::color blue = kl::color(0, 0, 255);
		inline const kl::color cyan = kl::color(30, 180, 170);
		inline const kl::color purple = kl::color(220, 0, 220);
		inline const kl::color yellow = kl::color(220, 220, 0);
		inline const kl::color orange = kl::color(255, 140, 0);
		inline const kl::color magenta = kl::color(155, 0, 155);
		inline const kl::color crimson = kl::color(100, 0, 0);
		inline const kl::color wheat = kl::color(245, 220, 180);
		inline const kl::color sky = kl::color(190, 245, 255);
	};
}
