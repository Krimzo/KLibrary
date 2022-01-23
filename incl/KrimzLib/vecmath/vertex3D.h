#pragma once 


namespace kl {
    struct vertex3D {
		kl::vec3 world;
		kl::vec2 texture;
		kl::vec3 normal;

		// Constructors
		vertex3D() {}
		vertex3D(kl::vec3 world) {
			this->world = world;
		}
		vertex3D(kl::vec3 world, kl::vec2 texture) {
			this->world = world;
			this->texture = texture;
		}
		vertex3D(kl::vec3 world, kl::vec2 texture, kl::vec3 normal) {
			this->world = world;
			this->texture = texture;
			this->normal = normal;
		}

		// Prints the vertex
		void print() {
			printf("% .2f % .2f % .2f | % .2f % .2f | % .2f % .2f % .2f\n", world.x, world.y, world.z, texture.x, texture.y, normal.x, normal.y, normal.z);
		}
	};
}