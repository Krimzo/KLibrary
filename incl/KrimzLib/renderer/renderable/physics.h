#pragma once


namespace kl {
	struct physics {
		bool enabled = false;
		float gravity = 9.81f;
		kl::vec3 velocity;
		kl::vec3 angular;
	};
}
