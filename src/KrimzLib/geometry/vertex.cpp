#include "KrimzLib/geometry/vertex.h"


// Constructors
kl::vertex::vertex() {

}
kl::vertex::vertex(const kl::float3& world) {
	this->world = world;
}
kl::vertex::vertex(const kl::float3& world, const kl::float2& texture) {
	this->world = world;
	this->texture = texture;
}
kl::vertex::vertex(const kl::float3& world, const kl::float3& normal) {
	this->world = world;
	this->normal = normal;
}
kl::vertex::vertex(const kl::float3& world, const kl::float2& texture, const kl::float3& normal) {
	this->world = world;
	this->texture = texture;
	this->normal = normal;
}

// Comparison
bool kl::vertex::equals(const kl::vertex& obj) const {
	return this->world == obj.world && this->texture == obj.texture && this->normal == obj.normal;
}
bool kl::vertex::operator==(const kl::vertex& obj) const {
	return this->equals(obj);
}
bool kl::vertex::operator!=(const kl::vertex& obj) const {
	return !this->equals(obj);
}
