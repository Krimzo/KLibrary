#include "KrimzLib/entity/entity.h"


// Constructor
kl::entity::entity(const std::string& name, ID3D11Buffer* mes, ID3D11ShaderResourceView* tex) {
	this->name = name;
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
void kl::entity::render(kl::gpu* gpu, bool useTex) const {
	// Binding the texture
	if (useTex) {
		gpu->bindPixlTex(texture, 0);
	}

	// Rendering
	gpu->draw(mesh);
}
