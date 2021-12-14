#pragma once


namespace kl {
	struct texture {
		// Constructor/destructor
		texture() {
			glGenTextures(1, &buffer);
		}
		~texture() {
			glDeleteTextures(1, &buffer);
		}

		// Loads the image data to the texture
		void load(kl::image& image) {
			image.flipVertical();
			glBindTexture(GL_TEXTURE_2D, buffer);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.getWidth(), image.getHeight(), 0, GL_BGR, GL_UNSIGNED_BYTE, image.pointer());
			glGenerateMipmap(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, NULL);
			image.flipVertical();
		}
		void load(kl::image&& image) {
			load(image);
		}

		// Binds the texture to the texture unit
		void bind(kl::id textureUnit) {
			glActiveTexture(textureUnit);
			glBindTexture(GL_TEXTURE_2D, buffer);
		}

	private:
		kl::id buffer;
	};
}
