#pragma once


namespace kl {
	struct colorf {
		float r;
		float g;
		float b;

		// Constructors
		colorf() {
			r = 0;
			g = 0;
			b = 0;
		}
		colorf(float r, float g, float b) {
			this->r = r;
			this->g = g;
			this->b = b;
		}
		colorf(kl::color color) {
			static float toFloatColor = 1.0f / 255;
			r = color.r * toFloatColor;
			g = color.g * toFloatColor;
			b = color.b * toFloatColor;
		}

		// Operator overloading
		bool operator == (kl::colorf obj) {
			return (r == obj.r && g == obj.g && b == obj.b);
		}
		bool operator != (kl::colorf obj) {
			return (r != obj.r || g != obj.g || b != obj.b);
		}
	};
}
