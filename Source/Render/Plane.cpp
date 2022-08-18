#include "Render/Plane.h"


kl::Plane::Plane() {}
kl::Plane::Plane(const Float3& normal, const Float3& point) : normal(normal.normalize()), point(point) {}

// std::cout
std::ostream& kl::operator<<(std::ostream& os, const Plane& obj) {
	os << "{" << obj.normal << ", " << obj.point << "}";
	return os;
}
