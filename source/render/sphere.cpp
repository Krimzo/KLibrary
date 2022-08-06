#include "render/sphere.h"


kl::Sphere::Sphere() {}
kl::Sphere::Sphere(const Float3& center, float radius) : center(center), radius(radius) {}

// std::cout
std::ostream& kl::operator<<(std::ostream& os, const kl::Sphere& obj) {
	os << "{" << obj.center << ", " << obj.radius << "}";
	return os;
}
