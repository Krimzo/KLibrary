#include "render/plane.h"


kl::Plane::Plane() {}
kl::Plane::Plane(const kl::Float3& normal, const kl::Float3& point) : normal(normal.normalize()), point(point) {}

// std::cout
std::ostream& kl::operator<<(std::ostream& os, const kl::Plane& obj) {
	os << "{" << obj.normal << ", " << obj.point << "}";
	return os;
}
