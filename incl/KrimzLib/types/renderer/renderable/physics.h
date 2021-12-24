#pragma once


namespace kl {
	struct physics {
		bool enabled = false;
		float gravity = 9.81;
		kl::vec3 velocity;
		kl::vec3 angular;
	};
}
