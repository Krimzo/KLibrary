#include "KrimzLib/geometry/sphere.h"


// Constructor
kl::sphere::sphere() {

}
kl::sphere::sphere(const kl::vec3& center, float radius, const kl::vec3& surfColor, const kl::vec3& emissColor = 0, float transparency = 0, float reflection = 0) {
    this->center = center;
    this->radius = radius;
    this->surfColor = surfColor;
    this->emissColor = emissColor;
    this->transparency = transparency;
    this->reflection = reflection;
}
