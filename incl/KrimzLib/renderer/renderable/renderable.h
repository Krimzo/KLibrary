#pragma once


namespace kl {
	struct renderable {
		// Object data
		bool visible = true;
		bool shadows = true;
		kl::geometry geometry;
		kl::physics physics;

		// Mesh pointer
		void* mesh = nullptr;

		// Texture pointer
		void* texture = nullptr;

		// Constructor
		renderable(kl::gl::mesh* mes, kl::gl::texture* tex) {
			mesh = mes;
			texture = tex;
		}
		renderable(kl::dx::mesh* mes, kl::dx::texture* tex) {
			mesh = mes;
			texture = tex;
		}

		// Renders a opengl mesh
		void gl_render() {
			// Binding the texture
			((kl::gl::texture*)texture)->bind();

			// Rendering
			((kl::gl::mesh*)mesh)->draw();
		}

		// Renders a directx mesh
		void dx_render(kl::dx::gpu* gpu) {
			// Binding the mesh
			gpu->bindMesh((kl::dx::mesh*)mesh);

			// Binding the texture
			gpu->bindTexture((kl::dx::texture*)texture, 0);

			// Rendering
			gpu->render(((kl::dx::mesh*)mesh)->vertCount());
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