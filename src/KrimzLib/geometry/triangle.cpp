#include "KrimzLib/geometry/triangle.h"


// Constructors
kl::triangle::triangle() {

}
kl::triangle::triangle(const kl::vec2& a, const kl::vec2& b, const kl::vec2& c) {
	this->a = a;
	this->b = b;
	this->c = c;
}

// Computes and stores the barycentric constants
void kl::triangle::calcConsts() {
	const float tempConst = 1 / ((b.y - c.y) * (a.x - c.x) + (c.x - b.x) * (a.y - c.y));
	interConsts = kl::vec4(
		(b.y - c.y) * tempConst,
		(c.x - b.x) * tempConst,
		(c.y - a.y) * tempConst,
		(a.x - c.x) * tempConst
	);
}

// Checks if the point is inside the triangle
bool kl::triangle::inTriangle(const kl::vec2& pos) const {
	kl::vec3 weights = getWeights(pos);
	return !(weights.x < 0 || weights.y < 0 || weights.z < 0);
}

// Interpolates and returns the depth
float kl::triangle::interpolate(const kl::vec3& values, const kl::vec2& pos) const {
	kl::vec3 weights = getWeights(pos);
	return values.x * weights.x + values.y * weights.y + values.z * weights.z;
}

// Calculates and returns the 3 barycentric weights of a triangle and a point
kl::vec3 kl::triangle::getWeights(const kl::vec2& pos) const {
	const float dx = pos.x - c.x;
	const float dy = pos.y - c.y;
	const float interWeight1 = dx * interConsts.x + dy * interConsts.y;
	const float interWeight2 = dx * interConsts.z + dy * interConsts.w;
	return kl::vec3(interWeight1, interWeight2, 1 - interWeight1 - interWeight2);
}
