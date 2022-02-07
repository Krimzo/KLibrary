#pragma once

#include <iostream>
#include <string>

#include "KrimzLib/gl/gl.h"
#include "KrimzLib/graphics/image.h"


namespace kl {
	class texture {
	public:
		// Constructors/destructor
		texture(const kl::image& image);
		texture(const kl::image& front, const kl::image& back, const kl::image& left, const kl::image& right, const kl::image& top, const kl::image& bottom);
		~texture();

		// Binds the texture to the texture unit
		void bind(kl::id textureUnit = GL_TEXTURE0) const;

	private:
		kl::id textureID = NULL;
		kl::id textureType = NULL;

		// Loads the image data to a texture
		void load2D(const kl::image& image);

		// Loads the 3d image data to a cubemap
		void load3D(const kl::image& front, const kl::image& back, const kl::image& left, const kl::image& right, const kl::image& top, const kl::image& bottom);
	};
}
