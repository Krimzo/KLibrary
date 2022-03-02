#include "KrimzLib/geometry/ray.h"


// Constructors
kl::ray::ray() {

}
kl::ray::ray(const kl::float3& origin, const kl::float3& direction) {
	this->origin = origin;
	this->direction = direction;
}

// Intersection with a plane
bool kl::ray::intersect(const kl::plane& plane, kl::float3* outInter) const {
    const float dnDot = this->direction.dot(plane.normal);
    if (dnDot != 0.0f) {
        if (outInter) *outInter = this->origin - this->direction * ((this->origin - plane.point).dot(plane.normal) / dnDot);
        return true;
    }
    return false;
}

// Intersection with a triangle
bool kl::ray::intersect(const kl::triangle& triangle, kl::float3* outInter) const {
    // Const epsilon
    const float bias = 1e-7f;

    // Calculating triangle edges
    const kl::float3 edge1 = triangle.b.world - triangle.a.world;
    const kl::float3 edge2 = triangle.c.world - triangle.a.world;

    // More calculations
    const kl::float3 h = this->direction.cross(edge2);
    const float a = edge1.dot(h);

    // Parallel check
    if (a > -bias && a < bias) {
        return false;
    }

    // More calculations
    const kl::float3 s = this->origin - triangle.a.world;
    const float f = 1.0f / a;
    const float u = f * s.dot(h);

    // More checks
    if (u < 0.0f || u > 1.0f) {
        return false;
    }

    // More calculations
    const kl::float3 q = s.cross(edge1);
    const float v = f * this->direction.dot(q);

    // More checks
    if (v < 0.0f || u + v > 1.0f) {
        return false;
    }

    // More calculations that I don't understand
    const float t = f * edge2.dot(q);

    // Ray intersection
    if (t > bias) {
        if (outInter) *outInter = this->origin + this->direction * t;
        return true;
    }

    // Line intersection but no ray intersection
    return false;
}

// Intersection with a sphere
bool kl::ray::intersect(const kl::sphere& sphere, kl::float3* outInter, float* outDis) const {
    // Ray sphere center ray
    const kl::float3 centerRay = sphere.center - this->origin;

    // Center ray and main ray direction dot product
    const float cdDot = centerRay.dot(this->direction);
    if (cdDot < 0.0f) {
        return false;
    }

    // Calculations I don't understand
    const float ccDot = centerRay.dot(centerRay) - cdDot * cdDot;
    const float rr = sphere.radius * sphere.radius;
    if (ccDot > rr) {
        return false;
    }

    // Intersect distance calculation
    const float thc = sqrt(rr - ccDot);
    const float dis0 = cdDot - thc;
    const float dis1 = cdDot + thc;

    // Origin in sphere test
    if (dis0 < 0.0f) {
        if (outInter) *outInter = this->origin + this->direction * dis1;
        if (outDis) *outDis = dis1;
    }
    else {
        if (outInter) *outInter = this->origin + this->direction * dis0;
        if (outDis) *outDis = dis0;
    }
    return true;
}
