#pragma once 


namespace kl {
    struct camera {
		kl::vec3 position;
		kl::vec3 rotation;

		// Constructors
		camera() {
			position = kl::vec3();
			rotation = kl::vec3();
		}
		camera(kl::vec3 position, kl::vec3 rotation) {
			this->position = position;
			this->rotation = rotation;
		}
	};
}