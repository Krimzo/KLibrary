#include "render/plane.h"


kl::plane::plane() {}
kl::plane::plane(const kl::float3& normal, const kl::float3& point) : normal(normal.normalize()), point(point) {}

// std::cout
std::ostream& kl::operator<<(std::ostream& os, const kl::plane& obj) {
	os << "{" << obj.normal << ", " << obj.point << "}";
	return os;
}
