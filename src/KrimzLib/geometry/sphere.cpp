#include "KrimzLib/geometry/sphere.h"


// Constructor
kl::sphere::sphere() {

}
kl::sphere::sphere(const kl::float3& center, float radius, const kl::float3& color, float reflectivity, float emission) {
    this->center = center;
    this->radius = radius;
    this->color = color;
    this->reflectivity = reflectivity;
    this->emission = emission;
}

// Calculates the emissive color
kl::float3 kl::sphere::calcEmiss() const {
    return color * emission;
}
