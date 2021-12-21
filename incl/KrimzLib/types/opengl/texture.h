#pragma once


namespace kl {
	struct texture {
		// Constructor/destructor
		texture(kl::image& image) {
			glGenTextures(1, &textureID);
			load(image);
		}
		texture(kl::image&& image) {
			glGenTextures(1, &textureID);
			load(image);
		}
		~texture() {
			glDeleteTextures(1, &textureID);
		}

		// Loads the image data to the texture
		void load(kl::image& image) {
			// Flipping the image verticaly
			image.flipVertical();

			// Binding the texture object
			glBindTexture(GL_TEXTURE_2D, textureID);

			// Setting the texture parameters
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

			// Loading the pixel data
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.getWidth(), image.getHeight(), 0, GL_BGR, GL_UNSIGNED_BYTE, image.pointer());

			// Generating the texture mipmaps
			glGenerateMipmap(GL_TEXTURE_2D);

			// Unbinding the texture object
			glBindTexture(GL_TEXTURE_2D, NULL);

			// Resetting the image
			image.flipVertical();
		}

		// Binds the texture to the texture unit
		void bind(kl::id textureUnit = GL_TEXTURE0) {
			// Activating the given image unit
			glActiveTexture(textureUnit);

			// Binding the texture object
			glBindTexture(GL_TEXTURE_2D, textureID);
		}

	private:
		kl::id textureID = NULL;
	};
}
