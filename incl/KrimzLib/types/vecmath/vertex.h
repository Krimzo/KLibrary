#pragma once 


namespace kl {
    struct vertex {
		kl::vec3 world = kl::vec3();
		kl::vec2 texture = kl::vec2();
		kl::vec3 normal = kl::vec3();

		// Constructors
		vertex() {}
		vertex(kl::vec3 world) {
			this->world = world;
		}
		vertex(kl::vec3 world, kl::vec2 texture) {
			this->world = world;
			this->texture = texture;
		}
		vertex(kl::vec3 world, kl::vec2 texture, kl::vec3 normal) {
			this->world = world;
			this->texture = texture;
			this->normal = normal;
		}

		// Prints the vertex
		void print() {
			printf("%.2f %.2f %.2f | %.2f %.2f | %.2f %.2f %.2f\n", world.x, world.y, world.z, texture.x, texture.y, normal.x, normal.y, normal.z);
		}
	};
}