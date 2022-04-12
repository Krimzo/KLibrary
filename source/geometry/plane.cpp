#include "geometry/plane.h"


kl::plane::plane()
{
}
kl::plane::plane(const kl::float3& normal, const kl::float3& point)
	: normal(normal.normalize()), point(point)
{
}
