#pragma once


namespace kl {
	struct texture {
		// Constructors/destructor
		texture(kl::image& image) {
			load2D(image);
		}
		texture(kl::image&& image) {
			load2D(image);
		}
		texture(kl::image& front, kl::image& back, kl::image& left, kl::image& right, kl::image& top, kl::image& bottom) {
			load3D(front, back, left, right, top, bottom);
		}
		texture(kl::image&& front, kl::image&& back, kl::image&& left, kl::image&& right, kl::image&& top, kl::image&& bottom) {
			load3D(front, back, left, right, top, bottom);
		}
		~texture() {
			glDeleteTextures(1, &textureID);
		}

		// Binds the texture to the texture unit
		void bind(kl::id textureUnit = GL_TEXTURE0) {
			// Activating the given image unit
			glActiveTexture(textureUnit);

			// Binding the texture object
			glBindTexture(textureType, textureID);
		}

	private:
		kl::id textureID = NULL;
		kl::id textureType = NULL;

		// Loads the image data to a texture
		void load2D(kl::image& image) {
			// Setting the texture type
			textureType = GL_TEXTURE_2D;

			// Flipping the image verticaly
			image.flipVertical();

			// Generating the texture
			glGenTextures(1, &textureID);

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

		// Loads the 3d image data to a cubemap
		void load3D(kl::image& front, kl::image& back, kl::image& left, kl::image& right, kl::image& top, kl::image& bottom) {
			// Setting the texture type
			textureType = GL_TEXTURE_CUBE_MAP;

			// Generating the texture
			glGenTextures(1, &textureID);

			// Binding the texture object
			glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

			// Setting the texture parameters
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

			// Loading the pixel data
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, right.getWidth(), right.getHeight(), 0, GL_BGR, GL_UNSIGNED_BYTE, right.pointer());
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, left.getWidth(), left.getHeight(), 0, GL_BGR, GL_UNSIGNED_BYTE, left.pointer());
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, top.getWidth(), top.getHeight(), 0, GL_BGR, GL_UNSIGNED_BYTE, top.pointer());
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, bottom.getWidth(), bottom.getHeight(), 0, GL_BGR, GL_UNSIGNED_BYTE, bottom.pointer());
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, front.getWidth(), front.getHeight(), 0, GL_BGR, GL_UNSIGNED_BYTE, front.pointer());
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, back.getWidth(), back.getHeight(), 0, GL_BGR, GL_UNSIGNED_BYTE, back.pointer());

			// Unbinding the texture object
			glBindTexture(GL_TEXTURE_CUBE_MAP, NULL);
		}
	};
}
