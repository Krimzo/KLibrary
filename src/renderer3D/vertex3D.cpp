#include "KrimzLib/renderer3D/vertex3D.h"


// Constructors
kl::vertex3D::vertex3D() {

}
kl::vertex3D::vertex3D(const kl::vec3& world) {
	this->world = world;
}
kl::vertex3D::vertex3D(const kl::vec3& world, const kl::vec2& texture) {
	this->world = world;
	this->texture = texture;
}
kl::vertex3D::vertex3D(const kl::vec3& world, const kl::vec2& texture, const kl::vec3& normal) {
	this->world = world;
	this->texture = texture;
	this->normal = normal;
}

// Comparison
bool kl::vertex3D::equals(const kl::vertex3D& obj) const {
	return this->world == obj.world && this->texture == obj.texture && this->normal == obj.normal;
}
bool kl::vertex3D::operator==(const kl::vertex3D& obj) const {
	return this->equals(obj);
}
bool kl::vertex3D::operator!=(const kl::vertex3D& obj) const {
	return !this->equals(obj);
}
