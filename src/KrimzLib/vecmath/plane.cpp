#include "KrimzLib/vecmath/plane.h"


kl::plane::plane() {

}
kl::plane::plane(const kl::vec3& normal, const kl::vec3& point) {
	this->normal = normal.normalize();
	this->point = point;
}
