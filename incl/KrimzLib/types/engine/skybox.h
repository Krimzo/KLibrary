#pragma once


namespace kl {
	struct skybox {
		skybox(kl::image& front, kl::image& back, kl::image& left, kl::image& right, kl::image& top, kl::image& bottom, std::string vertSource, std::string fragSource) {
			build(front, back, left, right, top, bottom, vertSource, fragSource);
		}
		skybox(kl::image&& front, kl::image&& back, kl::image&& left, kl::image&& right, kl::image&& top, kl::image&& bottom, std::string vertSource, std::string fragSource) {			
			build(front, back, left, right, top, bottom, vertSource, fragSource);
		}
		~skybox() {
			// Deleting the texture
			glDeleteTextures(1, &cubemapID);

			// Deleting the mesh
			delete boxMes;

			// Deleting the shaders
			delete boxSha;
		}

		// Renders the cubemap
		void render(kl::mat4& vpMat) {
			// Setting skybox uniforms
			vpUni.setData(vpMat);

			// Setting the depth testing
			glDepthFunc(GL_LEQUAL);

			// Drawing the cubemap
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapID);
			boxMes->draw();

			// Resetting the depth testing
			glDepthFunc(GL_LESS);
		}
		void render(kl::mat4&& vpMat) {
			render(vpMat);
		}

	private:
		kl::id cubemapID = NULL;
		kl::mesh* boxMes = nullptr;
		kl::shaders* boxSha = nullptr;
		kl::uniform vpUni;

		// Generates the box mesh
		void genBoxMes() {
			std::vector<kl::vertex> boxVertices = {
				kl::vertex(kl::vec3( 1, -1,  1)), kl::vertex(kl::vec3( 1, -1, -1)), kl::vertex(kl::vec3( 1,  1, -1)),
				kl::vertex(kl::vec3( 1,  1, -1)), kl::vertex(kl::vec3( 1,  1,  1)), kl::vertex(kl::vec3( 1, -1,  1)),
				kl::vertex(kl::vec3(-1, -1,  1)), kl::vertex(kl::vec3(-1,  1,  1)), kl::vertex(kl::vec3(-1,  1, -1)),
				kl::vertex(kl::vec3(-1,  1, -1)), kl::vertex(kl::vec3(-1, -1, -1)), kl::vertex(kl::vec3(-1, -1,  1)),
				kl::vertex(kl::vec3(-1,  1,  1)), kl::vertex(kl::vec3( 1,  1,  1)), kl::vertex(kl::vec3( 1,  1, -1)),
				kl::vertex(kl::vec3( 1,  1, -1)), kl::vertex(kl::vec3(-1,  1, -1)), kl::vertex(kl::vec3(-1,  1,  1)),
				kl::vertex(kl::vec3(-1, -1,  1)), kl::vertex(kl::vec3(-1, -1, -1)), kl::vertex(kl::vec3( 1, -1, -1)),
				kl::vertex(kl::vec3( 1, -1, -1)), kl::vertex(kl::vec3( 1, -1,  1)), kl::vertex(kl::vec3(-1, -1,  1)),
				kl::vertex(kl::vec3(-1, -1,  1)), kl::vertex(kl::vec3( 1, -1,  1)), kl::vertex(kl::vec3( 1,  1,  1)),
				kl::vertex(kl::vec3( 1,  1,  1)), kl::vertex(kl::vec3(-1,  1,  1)), kl::vertex(kl::vec3(-1, -1,  1)),
				kl::vertex(kl::vec3(-1, -1, -1)), kl::vertex(kl::vec3(-1,  1, -1)), kl::vertex(kl::vec3( 1,  1, -1)),
				kl::vertex(kl::vec3( 1,  1, -1)), kl::vertex(kl::vec3( 1, -1, -1)), kl::vertex(kl::vec3(-1, -1, -1))
			};
			boxMes = new kl::mesh(boxVertices);
		}

		// Generates the cubemap
		void genCubemap() {
			glGenTextures(1, &cubemapID);
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapID);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
		}

		// Builds the skybox
		void build(kl::image& front, kl::image& back, kl::image& left, kl::image& right, kl::image& top, kl::image& bottom, std::string vertSource, std::string fragSource) {
			// Compiling skybox shaders
			boxSha = new shaders(vertSource, fragSource);

			// Getting the view/projection uniform
			vpUni = boxSha->getUniform("vp");

			// Generating the box vertices
			genBoxMes();

			// Creating the cubemap
			genCubemap();

			// Loading the pixel data
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, right.getWidth(), right.getHeight(), 0, GL_BGR, GL_UNSIGNED_BYTE, right.pointer());
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, left.getWidth(), left.getHeight(), 0, GL_BGR, GL_UNSIGNED_BYTE, left.pointer());
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, top.getWidth(), top.getHeight(), 0, GL_BGR, GL_UNSIGNED_BYTE, top.pointer());
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, bottom.getWidth(), bottom.getHeight(), 0, GL_BGR, GL_UNSIGNED_BYTE, bottom.pointer());
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, front.getWidth(), front.getHeight(), 0, GL_BGR, GL_UNSIGNED_BYTE, front.pointer());
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, back.getWidth(), back.getHeight(), 0, GL_BGR, GL_UNSIGNED_BYTE, back.pointer());
		}
	};
}
