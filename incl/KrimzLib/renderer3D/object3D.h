#pragma once


namespace kl {
	struct object3D {
		// View
		bool visible = true;
		bool shadows = true;

		// Geometry
		kl::vec3 size = kl::vec3(1);
		kl::vec3 rotation;
		kl::vec3 position;
		
		// Physics
		bool physics = true;
		kl::vec3 acceler;
		kl::vec3 velocity;
		kl::vec3 angular;

		// Mesh pointer
		kl::mesh* mesh = nullptr;

		// Texture pointer
		kl::texture* texture = nullptr;

		// Constructor
		object3D(kl::mesh* mes, kl::texture* tex) {
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
		kl::mat4 matrix() {
			return kl::mat4::translate(position) * kl::mat4::rotate(rotation) * kl::mat4::scale(size);
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