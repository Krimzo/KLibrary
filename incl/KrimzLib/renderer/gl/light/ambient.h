#pragma once


namespace kl {
	namespace gl {
		struct ambient {
			kl::vec3 color;
			float intensity = 0;

			// Returns the true light color
			kl::vec3 getCol() {
				return color * intensity;
			}
		};
	}
}
