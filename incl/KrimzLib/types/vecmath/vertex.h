#pragma once 


namespace kl {
    struct vertex {
		kl::vec3 world;
		kl::vec2 texture;
		kl::colorf color;

		// Constructors
		vertex() {
			world = kl::vec3();
			texture = kl::vec2();
			color = kl::color();
		}
		vertex(kl::vec3 world) {
			this->world = world;
			texture = kl::vec2();
			color = kl::color();
		}
		vertex(kl::vec3 world, kl::vec2 texture) {
			this->world = world;
			this->texture = texture;
			color = kl::color();
		}
		vertex(kl::vec3 world, kl::colorf color) {
			this->world = world;
			texture = kl::vec2();
			this->color = color;
		}
		vertex(kl::vec3 world, kl::vec2 texture, kl::colorf color) {
			this->world = world;
			this->texture = texture;
			this->color = color;
		}
	};
}