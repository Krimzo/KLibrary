#pragma once


namespace kl {
	struct texture {
		// Constructor/destructor
		texture() {
			textureID = NULL;
		}
		~texture() {

		}

	private:
		kl::id textureID;
	};
}
