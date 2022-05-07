#include "geometry/sphere.h"


// Constructor
kl::sphere::sphere() {}
kl::sphere::sphere(const kl::float3& center, float radius, const kl::float3& color, float reflectivity, float emission) : center(center), radius(radius), color(color), reflectivity(reflectivity), emission(emission) {}

// Calculates the emissive color
kl::float3 kl::sphere::calcEmiss() const {
	return color * emission;
}

// Overloading std::cout
std::ostream& kl::operator<<(std::ostream& os, const kl::sphere& obj) {
	os << "{" << obj.center << ", " << obj.radius << "}";
	return os;
}
