#pragma once


namespace kl {
	struct renderable {
		// Object data
		bool visible = true;
		bool shadows = true;
		kl::geometry geometry;
		kl::physics physics;

		// Mesh pointers
		kl::gl::mesh* gl_mesh = nullptr;
		kl::dx::mesh* dx_mesh = nullptr;

		// Texture pointers
		kl::gl::texture* gl_tex = nullptr;
		kl::dx::texture* dx_tex = nullptr;

		// Constructor
		renderable(kl::gl::mesh* mesh, kl::gl::texture* tex) {
			gl_mesh = mesh;
			gl_tex = tex;
		}
		renderable(kl::dx::mesh* mesh, kl::dx::texture* tex) {
			dx_mesh = mesh;
			dx_tex = tex;
		}

		// Renders a opengl mesh
		void gl_render() {
			gl_tex->bind();
			gl_mesh->draw();
		}

		// Renders a directx mesh
		void dx_render(kl::dx::gpu* dx_gpu) {
			// Binding the mesh
			dx_gpu->bindMesh(dx_mesh);

			// Binding the texture
			dx_gpu->bindTexture(dx_tex, 0);

			// Rendering
			dx_gpu->render(dx_mesh->vertCount());
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