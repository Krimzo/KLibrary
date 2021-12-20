#pragma once


namespace kl {
    struct gameobject {
		// Object data
		bool visible = true;
		kl::geometry geometry = kl::geometry();
		kl::physics physics = kl::physics();
		
		// Buffer pointers
		kl::mesh* mesh = nullptr;
		kl::texture* texture = nullptr;

		// Renders the game object
		void render() {
			texture->bind();
			mesh->draw();
		}
	};
}