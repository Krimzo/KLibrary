#include "render/vertex.h"


kl::vertex::vertex()
{
}
kl::vertex::vertex(const kl::float3& world) : world(world)
{
}
kl::vertex::vertex(const kl::float3& world, const kl::float2& texture) : world(world), texture(texture)
{
}
kl::vertex::vertex(const kl::float3& world, const kl::float3& normal) : world(world), normal(normal)
{
}
kl::vertex::vertex(const kl::float3& world, const kl::float2& texture, const kl::float3& normal) : world(world), texture(texture), normal(normal)
{
}

// std::cout
std::ostream& kl::operator<<(std::ostream& os, const kl::vertex& obj)
{
	os << "{" << obj.world << ", " << obj.texture << ", " << obj.normal << "}";
	return os;
}
