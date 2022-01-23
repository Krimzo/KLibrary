#pragma once 


namespace kl {
	struct vertex2D {
		kl::vec2 world;
		kl::vec2 texture;

		// Constructors
		vertex2D() {}
		vertex2D(kl::vec2 world) {
			this->world = world;
		}
		vertex2D(kl::vec2 world, kl::vec2 texture) {
			this->world = world;
			this->texture = texture;
		}

		// Prints the vertex
		void print() {
			printf("% .2f % .2f | % .2f % .2f\n", world.x, world.y, texture.x, texture.y);
		}
	};
}