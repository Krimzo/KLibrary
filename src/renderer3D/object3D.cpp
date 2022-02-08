#include "KrimzLib/renderer3D/object3D.h"


// Constructor
kl::object3D::object3D(kl::mesh* mes, kl::texture* tex) {
	mesh = mes;
	texture = tex;
}

// Updates the object physics
void kl::object3D::upPhys(float deltaT) {
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
kl::mat4 kl::object3D::matrix() const {
	return kl::mat4::translate(position) * kl::mat4::rotate(rotation) * kl::mat4::scale(size);
}

// Renders the mesh
void kl::object3D::render() const {
	// Binding the texture
	texture->bind();

	// Rendering
	mesh->draw();
}
