#pragma once 

#include <iostream>
#include <windows.h>

#include "cuda/cu_types.h"


using byte = unsigned char;

namespace kl {
	class color {
	public:
		byte b, g, r, a;

		ALL color() : b(0), g(0), r(0), a(0) {}
		ALL color(byte r, byte g, byte b, byte a = 255) : b(b), g(g), r(r), a(a) {}

		ALL bool equals(const kl::color& obj) const {
			return r == obj.r && g == obj.g && b == obj.b && a == obj.a;
		}
		ALL bool operator==(const kl::color& obj) const {
			return equals(obj);
		}
		ALL bool operator!=(const kl::color& obj) const {
			return !equals(obj);
		}

		ALL kl::color gray() const {
			const byte grayValue = byte(r * 0.3f + g * 0.585f + b * 0.115f);
			return kl::color(grayValue, grayValue, grayValue, a);
		}
		ALL kl::color invert() const {
			return kl::color(255 - r, 255 - g, 255 - b, a);
		}
		ALL char ascii() const {
			static const char asciiTable[10] = { '@', '%', '#', 'x', '+', '=', ':', '-', '.', ' ' };
			static const float toSatur = 9.0f / 255.0f;
			return asciiTable[int(gray().r * toSatur)];
		}

		ALL kl::color mix(const kl::color& col, float ratio) const {
			ratio = min(max(ratio, 0.0f), 1.0f);
			const float iratio = 1.0f - ratio;
			return kl::color(
				byte(r * iratio) + byte(col.r * ratio),
				byte(g * iratio) + byte(col.g * ratio),
				byte(b * iratio) + byte(col.b * ratio)
			);
		}
		ALL kl::color mix(const kl::color& col) const {
			static const float toFloatCol = 1.0f / 255;
			return mix(col, col.a * toFloatCol);
		}
	};

	// std::cout
	inline std::ostream& operator<<(std::ostream& os, const kl::color& obj) {
		os << "\033[38;2;" << int(obj.r) << ";" << int(obj.g) << ";" << int(obj.b) << "m";
		return os;
	}

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
