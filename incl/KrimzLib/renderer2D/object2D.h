#pragma once


namespace kl {
	struct object2D {
		// View
		bool visible = true;

		// Geometry
		kl::vec2 size = kl::vec2(1);
		kl::vec2 position;
		float rotation = 0;
		
		// Physics
		bool physics = true;
		kl::vec2 acceler;
		kl::vec2 velocity;
		float angular = 0;

		// Mesh pointer
		kl::mesh* mesh = nullptr;

		// Texture pointer
		kl::texture* texture = nullptr;

		// Constructor
		object2D(kl::mesh* mes, kl::texture* tex) {
			mesh = mes;
			texture = tex;
		}

		// Updates the object physics
		void upPhys(float deltaT) {
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
		kl::mat3 matrix() {
			return kl::mat3::translate(position) * kl::mat3::rotate(rotation) * kl::mat3::scale(size);
		}

		// Renders the mesh
		void render() {
			// Binding the texture
			texture->bind();

			// Rendering
			mesh->draw();
		}
	};
}