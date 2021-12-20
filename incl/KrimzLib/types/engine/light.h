#pragma once


namespace kl {
	struct light {
		kl::vec3 color = kl::vec3(1, 1, 1);
		kl::vec3 position = kl::vec3();
		kl::vec3 rotation = kl::vec3();
		float intensity = 1;

		// Constructors
		light() {}
		light(kl::vec3 color) {
			this->color = color;
		}
		light(float intensity) {
			this->intensity = intensity;
		}
		light(kl::vec3 color, float intensity) {
			this->color = color;
			this->intensity = intensity;
		}

		// Returns the true light color
		kl::vec3 getColor() {
			return color * intensity;
		}
	};
}
