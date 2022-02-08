#pragma once

#include <iostream>
#include <string>

#include "KrimzLib/gl/gl.h"
#include "KrimzLib/graphics/image.h"


namespace kl {
	class texture {
	private:
		kl::id textureID = NULL;
		kl::id textureType = NULL;

	public:
		// Constructors/destructor
		texture(const kl::image& image);
		texture(const kl::image& front, const kl::image& back, const kl::image& left, const kl::image& right, const kl::image& top, const kl::image& bottom);
		~texture();

		// Binds the texture to the texture unit
		void bind(kl::id textureUnit = GL_TEXTURE0) const;
	};
}
