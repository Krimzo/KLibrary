#pragma once 


namespace kl {
    struct vertex {
		kl::vec3 world;
		kl::vec2 texture;
		kl::color color;

		// Constructors
		vertex() {
			world = {};
			texture = {};
			color = {};
		}
		vertex(kl::vec3 world) {
			this->world = world;
			texture = {};
			color = {};
		}
		vertex(kl::vec3 world, kl::color color) {
			this->world = world;
			this->texture = {};
			this->color = color;
		}
		vertex(kl::vec3 world, kl::vec2 texture) {
			this->world = world;
			this->texture = texture;
			color = {};
		}
	};
}