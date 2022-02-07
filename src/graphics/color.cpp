#include "KrimzLib/graphics/color.h"


// Constructors
kl::color::color() {
	r = 0;
	g = 0;
	b = 0;
}
kl::color::color(byte r, byte g, byte b) {
	this->r = r;
	this->g = g;
	this->b = b;
}

// Operator overloading
bool kl::color::operator == (kl::color obj) const {
	return (r == obj.r && g == obj.g && b == obj.b);
}
bool kl::color::operator != (kl::color obj) const {
	return (r != obj.r || g != obj.g || b != obj.b);
}

// Prints the color
void kl::color::print() const {
	printf("%d %d %d\n", r, g, b);
}
