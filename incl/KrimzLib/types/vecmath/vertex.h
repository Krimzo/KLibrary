#pragma once 


namespace kl {
    struct vertex {
		kl::vec3 world;
		kl::vec2 texture;

		// Constructors
		vertex() {
			world = kl::vec3();
			texture = kl::vec2();
		}
		vertex(kl::vec3 world) {
			this->world = world;
			texture = kl::vec2();
		}
		vertex(kl::vec2 texture) {
			world = kl::vec3();
			this->texture = texture;
		}
		vertex(kl::vec3 world, kl::vec2 texture) {
			this->world = world;
			this->texture = texture;
		}

		// Prints the vertex
		void print() {
			printf("%.2f %.2f %.2f | %.2f %.2f\n", world.x, world.y, world.z, texture.x, texture.y);
		}
	};
}