#include "render/vertex.h"


kl::Vertex::Vertex() {}
kl::Vertex::Vertex(const Float3& world) : world(world) {}
kl::Vertex::Vertex(const Float3& world, const Float2& texture) : world(world), texture(texture) {}
kl::Vertex::Vertex(const Float3& world, const Float3& normal) : world(world), normal(normal) {}
kl::Vertex::Vertex(const Float3& world, const Float2& texture, const Float3& normal) : world(world), texture(texture), normal(normal) {}

// std::cout
std::ostream& kl::operator<<(std::ostream& os, const Vertex& obj) {
	os << "{" << obj.world << ", " << obj.texture << ", " << obj.normal << "}";
	return os;
}
