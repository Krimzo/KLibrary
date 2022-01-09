#pragma once


namespace kl {
	struct renderable {
		// Object data
		bool visible = true;
		bool shadows = true;
		kl::geometry geometry;
		kl::physics physics;

		// Mesh pointer
		kl::mesh* mesh = nullptr;

		// Texture pointer
		kl::texture* texture = nullptr;

		// Constructor
		renderable(kl::mesh* mes, kl::texture* tex) {
			mesh = mes;
			texture = tex;
		}

		// Renders the mesh
		void render() {
			// Binding the texture
			texture->bind();

			// Rendering
			mesh->draw();
		}

		// Updates the object physics
		void upPhys(float deltaT) {
			if (physics.enabled) {
				// Applying gravity
				physics.velocity.y -= physics.gravity * deltaT;

				// Applying velocity
				geometry.position += physics.velocity * deltaT;

				// Applying angular momentum
				geometry.rotation += physics.angular * deltaT;
			}
		}
	};
}