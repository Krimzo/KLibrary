#pragma once


namespace kl {
	struct physics {
		bool enabled = false;
		float gravity = 1;
		kl::vec3 velocity = kl::vec3();
		kl::vec3 angular = kl::vec3();
	};
}
