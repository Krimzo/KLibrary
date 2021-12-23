#pragma once


namespace kl {
	struct direct {
		kl::vec3 color;
		kl::vec3 direction;
		float intensity = 0;

		// Returns the true light color
		kl::vec3 getColor() {
			return color * intensity;
		}

		// Returns the direction of light
		kl::vec3 getDirection() {
			return direction.normalize();
		}
	};
}
