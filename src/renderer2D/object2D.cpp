#include "KrimzLib/renderer2D/object2D.h"


// Constructor
kl::object2D::object2D(kl::mesh* mes, kl::texture* tex) {
	mesh = mes;
	texture = tex;
}

// Updates the object physics
void kl::object2D::upPhys(float deltaT) {
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
kl::mat3 kl::object2D::matrix() const {
	return kl::mat3::translate(position) * kl::mat3::rotate(rotation) * kl::mat3::scale(size);
}

// Renders the mesh
void kl::object2D::render() const {
	// Binding the texture
	texture->bind();

	// Rendering
	mesh->draw();
}
