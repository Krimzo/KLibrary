#include "KrimzLib/vecmath/ray.h"


// Constructors
kl::ray::ray() {

}
kl::ray::ray(const kl::vec3& origin, const kl::vec3& direction) {
	this->origin = origin;
	this->direction = direction;
}

// Intersection with a plane
kl::vec3 kl::ray::intersect(const kl::plane& plane) const {
	return this->origin - this->direction * ((this->origin - plane.point).dot(plane.normal) / this->direction.dot(plane.normal));
}

// Intersection with a triangle
bool kl::ray::intersect(const kl::triangle& triangle, kl::vec3* outIntersect) const {
    // Const epsilon
    const float EPSILON = 1e-7f;

    // Calculating triangle edges
    const kl::vec3 edge1 = triangle.b.world - triangle.a.world;
    const kl::vec3 edge2 = triangle.c.world - triangle.a.world;

    // More calculations
    const kl::vec3 h = this->direction.cross(edge2);
    const float a = edge1.dot(h);

    // Parallel check
    if (a > -EPSILON && a < EPSILON) {
        return false;
    }

    // More calculations
    const kl::vec3 s = this->origin - triangle.a.world;
    const float f = 1.0f / a;
    const float u = f * s.dot(h);

    // More checks
    if (u < 0.0f || u > 1.0f) {
        return false;
    }

    // More calculations
    const kl::vec3 q = s.cross(edge1);
    const float v = f * this->direction.dot(q);

    // More checks
    if (v < 0.0f || u + v > 1.0f) {
        return false;
    }

    // More calculations that I don't understand
    const float t = f * edge2.dot(q);

    // Ray intersection
    if (t > EPSILON) {
        *outIntersect = this->origin + this->direction * t;
        return true;
    }
    // Line intersection but no ray intersection
    else {
        return false;
    }
}
