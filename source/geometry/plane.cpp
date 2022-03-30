#include "geometry/plane.h"


kl::plane::plane() {

}
kl::plane::plane(const kl::float3& normal, const kl::float3& point) {
	this->normal = normal.normalize();
	this->point = point;
}
