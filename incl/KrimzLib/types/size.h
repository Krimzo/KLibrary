#pragma once


namespace kl {
    struct size {
		kl::uint32 width;
		kl::uint32 height;

		// Constructors
		size() {
			width = 0;
			height = 0;
		}
		size(kl::uint32 width, kl::uint32 height) {
			this->width = width;
			this->height = height;
		}

		// Operator overloading
		bool operator == (kl::size obj) {
			return (width == obj.width && height == obj.height);
		}
		bool operator != (kl::size obj) {
			return (width != obj.width || height != obj.height);
		}
	};
}