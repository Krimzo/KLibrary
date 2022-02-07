#include "KrimzLib/renderer2D/vertex2D.h"


// Constructors
kl::vertex2D::vertex2D() {

}
kl::vertex2D::vertex2D(const kl::vec2& world) {
	this->world = world;
}
kl::vertex2D::vertex2D(const kl::vec2& world, const kl::vec2& texture) {
	this->world = world;
	this->texture = texture;
}

// Comparison
bool kl::vertex2D::equals(const kl::vertex2D& obj) const {
	return this->world == obj.world && this->texture == obj.texture;
}
bool kl::vertex2D::operator==(const kl::vertex2D& obj) const {
	return this->equals(obj);
}
bool kl::vertex2D::operator!=(const kl::vertex2D& obj) const {
	return !this->equals(obj);
}

// Prints the vertex
void kl::vertex2D::print() const {
	printf("% .2f % .2f | % .2f % .2f\n", world.x, world.y, texture.x, texture.y);
}
