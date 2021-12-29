#pragma once


namespace kl {
	namespace gl {
		struct geometry {
			kl::vec3 size = kl::vec3(1, 1, 1);
			kl::vec3 rotation = kl::vec3();
			kl::vec3 position = kl::vec3();

			// Returns the world matrix
			kl::mat4 matrix() {
				return kl::mat4::translate(position) * kl::mat4::rotate(rotation) * kl::mat4::scale(size);
			}
		};
	}
}
