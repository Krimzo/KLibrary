#pragma once


namespace kl {
    struct renderable {
		// Object data
		bool visible = true;
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
				geometry.position.x += physics.velocity.x * deltaT;
				geometry.position.y += physics.velocity.y * deltaT;
				geometry.position.z += physics.velocity.z * deltaT;

				// Applying angular momentum
				geometry.rotation.x += physics.angular.x * deltaT;
				geometry.rotation.y += physics.angular.y * deltaT;
				geometry.rotation.z += physics.angular.z * deltaT;
			}
		}

		// Renders the game object
		void render() {
			texture->bind();
			mesh->draw();
		}
	};
}