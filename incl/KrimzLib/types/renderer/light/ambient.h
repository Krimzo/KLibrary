#pragma once


namespace kl {
	struct ambient {
		kl::vec3 color;
		float intensity = 0;

		// Returns the true light color
		kl::vec3 getColor() {
			return color * intensity;
		}
	};
}
