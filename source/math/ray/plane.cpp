#include "math/ray/plane.h"

#include "math/math.h"


kl::plane::plane()
{}

kl::plane::plane(const float3& normal, const float3& point)
    : normal(math::normalize(normal)), point(point)
{}

std::ostream& kl::operator<<(std::ostream& os, const kl::plane& obj)
{
    os << "{" << obj.normal << ", " << obj.point << "}";
    return os;
}
