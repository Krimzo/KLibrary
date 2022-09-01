#include "Render/Ray.h"


kl::Ray::Ray() {}

kl::Ray::Ray(const Float3& origin, const Float3& direction) : origin(origin), direction(direction.normalize()) {}

kl::Ray::Ray(const Float3& origin, const Mat4& inverseMatrix, const Float2& NDC) : origin(origin) {
	const Float4 pixelDirection = inverseMatrix * Float4(NDC, 1.0f, 1.0f);
	direction = (pixelDirection / pixelDirection.w).xyz.normalize();
}

kl::Ray::Ray(const Camera& camera, const Float2& NDC) : Ray(camera.position, camera.matrix().inverse(), NDC) {}

bool kl::Ray::intersectPlane(const Plane& plane, Float3* outIntersection) const {
	const float denom = plane.normal.normalize().dot(direction.normalize());
	if (std::abs(denom) > 0.0001f) {
		const float t = (plane.point - origin).dot(plane.normal) / denom;
		if (t >= 0.0f) {
			if (outIntersection) {
				*outIntersection = origin + direction * t;
			}
			return true;
		}
	}
	return false;
}

bool kl::Ray::intersectTriangle(const Triangle& triangle, Float3* outIntersection) const {
	const Float3 edge1 = triangle.b.world - triangle.a.world;
	const Float3 edge2 = triangle.c.world - triangle.a.world;

	const Float3 h = direction.cross(edge2);
	const Float3 s = origin - triangle.a.world;
	const float f = 1.0f / edge1.dot(h);
	const float u = s.dot(h) * f;
	if (u < 0.0f || u > 1.0f) {
		return false;
	}

	const Float3 q = s.cross(edge1);
	const float v = direction.dot(q) * f;
	if (v < 0.0f || (u + v) > 1.0f) {
		return false;
	}

	const float t = edge2.dot(q) * f;
	if (t > 0.0f) {
		if (outIntersection) {
			*outIntersection = origin + direction * t;
		}
		return true;
	}
	return false;
}

bool kl::Ray::intersectSphere(const Sphere& sphere, Float3& outIntersection, float* outDistance) const {
	const Float3 centerRay = sphere.center - origin;
	const float cdDot = centerRay.dot(direction);
	if (cdDot < 0.0f) {
		return false;
	}

	const float ccDot = centerRay.dot(centerRay) - cdDot * cdDot;
	const float rr = sphere.radius * sphere.radius;
	if (ccDot > rr) {
		return false;
	}

	const float thc = sqrt(rr - ccDot);
	const float dis0 = cdDot - thc;
	const float dis1 = cdDot + thc;

	outIntersection = origin + direction * ((dis0 < 0.0f) ? dis1 : dis0);

	if (outDistance) {
		*outDistance = (dis0 < 0.0f) ? dis1 : dis0;
	}

	return true;
}

bool kl::Ray::intersectSphere(const Sphere& sphere) const {
	const float rayDis = (sphere.center - origin).dot(direction);
	const Float3 rayPoint = origin + direction * rayDis;
	const float sphRayDis = (sphere.center - rayPoint).length();
	return !(sphRayDis > sphere.radius);
}

// std::cout
std::ostream& kl::operator<<(std::ostream& os, const Ray& obj) {
	os << "{" << obj.origin << ", " << obj.direction << "}";
	return os;
}
