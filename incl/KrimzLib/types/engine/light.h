#pragma once


namespace kl {
	struct light {
		kl::vec3 color = kl::vec3();
		kl::vec3 position = kl::vec3();
		kl::vec3 direction = kl::vec3();
		float intensity = 0;

		// Returns the true light color
		kl::vec3 getLight() {
			return color * intensity;
		}

		// Returns the direction of light
		kl::vec3 getDirection() {
			return direction.normalize();
		}
	};
}
