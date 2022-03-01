#include "KrimzLib/geometry/sphere.h"


// Constructor
kl::sphere::sphere() {

}
kl::sphere::sphere(const kl::vec3& center, float radius, const kl::vec3& color, float reflectivity, float emission) {
    this->center = center;
    this->radius = radius;
    this->color = color;
    this->reflectivity = reflectivity;
    this->emission = emission;
}

// Calculates the emissive color
kl::vec3 kl::sphere::calcEmiss() const {
    return color * emission;
}
