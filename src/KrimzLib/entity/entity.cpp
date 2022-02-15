#include "KrimzLib/entity/entity.h"


// Constructor
kl::entity::entity(kl::mesh* mes, kl::texture* tex) {
	mesh = mes;
	texture = tex;
}

// Updates the object physics
void kl::entity::upPhys(float deltaT) {
	if (physics) {
		// Applying acceleration
		velocity += acceler * deltaT;

		// Applying velocity
		position += velocity * deltaT;

		// Applying angular momentum
		rotation += angular * deltaT;
	}
}

// Returns the world matrix
kl::mat4 kl::entity::matrix() const {
	return kl::mat4::translate(position) * kl::mat4::rotate(rotation) * kl::mat4::scale(size);
}

// Renders the mesh
void kl::entity::render() const {
	// Binding the texture
	texture->bind(0);

	// Rendering
	mesh->draw();
}
