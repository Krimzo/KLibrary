#pragma once


namespace kl {
	namespace dx {
		struct skybox {
			skybox(kl::image& front, kl::image& back, kl::image& left, kl::image& right, kl::image& top, kl::image& bottom) {
				build(front, back, left, right, top, bottom);
			}
			~skybox() {

			}

		private:

			// Builds the skybox
			void build(kl::image& front, kl::image& back, kl::image& left, kl::image& right, kl::image& top, kl::image& bottom) {
				// Generating the box mesh


				// Generating the box texture


				// Compiling skybox shaders

			}
		};
	}
}
