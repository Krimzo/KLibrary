#pragma once


namespace kl {
	struct texture {
		// Constructor/destructor
		texture(kl::image& image, bool flipY) {
			glGenTextures(1, &textureID);
			load(image, flipY);
		}
		texture(kl::image&& image, bool flipY) {
			glGenTextures(1, &textureID);
			load(image, flipY);
		}
		~texture() {
			glDeleteTextures(1, &textureID);
		}

		// Loads the image data to the texture
		void load(kl::image& image, bool flipY) {
			if (flipY) image.flipVertical();
			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.getWidth(), image.getHeight(), 0, GL_BGR, GL_UNSIGNED_BYTE, image.pointer());
			glGenerateMipmap(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, NULL);
			if (flipY) image.flipVertical();
		}
		void load(kl::image&& image, bool flipY) {
			load(image, flipY);
		}

		// Binds the texture to the texture unit
		void bind(kl::id textureUnit = GL_TEXTURE0) {
			glActiveTexture(textureUnit);
			glBindTexture(GL_TEXTURE_2D, textureID);
		}

	private:
		kl::id textureID = NULL;
	};
}
