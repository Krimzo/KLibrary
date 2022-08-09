#include "Render\Entity.h"


kl::Entity::Entity() {}

void kl::Entity::updatePhysics(float deltaTime) {
	velocity += acceleration * deltaTime;
	position += velocity * deltaTime;
	rotation += angular * deltaTime;
}

kl::Mat4 kl::Entity::matrix() const {
	return Mat4::Translation(position) * Mat4::Rotation(rotation) * Mat4::Scaling(scale);
}
