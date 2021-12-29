#pragma once


namespace kl {
	namespace gl {
		struct renderable {
			// Object data
			bool visible = true;
			bool shadows = true;
			kl::gl::geometry geometry;
			kl::gl::physics physics;

			// Buffer pointers
			kl::gl::mesh* mesh = nullptr;
			kl::gl::texture* texture = nullptr;

			// Constructor
			renderable(kl::gl::mesh* mes, kl::gl::texture* tex) {
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
}