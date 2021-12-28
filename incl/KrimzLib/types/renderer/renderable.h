#pragma once


namespace kl {
    struct renderable {
		// Object data
		bool visible = true;
		bool shadows = true;
		kl::geometry geometry;
		kl::physics physics;
		
		// Buffer pointers
		kl::mesh* mesh = nullptr;
		kl::texture* texture = nullptr;

		// Constructor
		renderable(kl::mesh* mes, kl::texture* tex) {
			this->mesh = mes;
			this->texture = tex;
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

		// Renders the game object
		void render() {
			texture->bind();
			mesh->draw();
		}
	};
}