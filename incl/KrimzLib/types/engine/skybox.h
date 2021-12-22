#pragma once


namespace kl {
	struct skybox {
		skybox(kl::image& front, kl::image& back, kl::image& left, kl::image& right, kl::image& top, kl::image& bottom) {
			build(front, back, left, right, top, bottom);
		}
		skybox(kl::image&& front, kl::image&& back, kl::image&& left, kl::image&& right, kl::image&& top, kl::image&& bottom) {			
			build(front, back, left, right, top, bottom);
		}
		~skybox() {
			// Deleting the texture
			delete boxTex;

			// Deleting the mesh
			delete boxMes;

			// Deleting the shaders
			delete boxSha;
		}

		// Renders the cubemap
		void render(kl::mat4& vpMat) {
			// Setting the depth testing
			glDepthFunc(GL_LEQUAL);

			// Setting skybox uniforms
			vp_uni.setData(vpMat);

			// Drawing the cubemap
			boxTex->bind();
			boxMes->draw();

			// Resetting the depth testing
			glDepthFunc(GL_LESS);
		}
		void render(kl::mat4&& vpMat) {
			render(vpMat);
		}

	private:
		kl::mesh* boxMes = nullptr;
		kl::texture* boxTex = nullptr;
		kl::shaders* boxSha = nullptr;
		kl::uniform vp_uni;

		// Box vertices
		static std::vector<kl::vertex> boxVertices;

		// Skybox shader sources
		static std::string vertSource;
		static std::string fragSource;

		// Builds the skybox
		void build(kl::image& front, kl::image& back, kl::image& left, kl::image& right, kl::image& top, kl::image& bottom) {
			// Generating the box mesh
			boxMes = new kl::mesh(boxVertices);

			// Generating the box texture
			boxTex = new kl::texture(front, back, left, right, top, bottom);

			// Compiling skybox shaders
			boxSha = new shaders(vertSource, fragSource);

			// Getting the view/projection uniform
			vp_uni = boxSha->getUniform("vp");
		}
	};

	// Skybox vertex shader
	std::string kl::skybox::vertSource = R"(
			#version 330

			// Input arrays
			layout (location = 0) in vec3 world;

			// Transformation matrices
			uniform mat4 vp;

			// Interpolated values output
			out vec3 interTex;

			void main() {
				// Outputting the transformed world coords
				gl_Position = (vp * vec4(world, 0)).xyww;
    
				// Outputting the interpolated texture coords
				interTex = world;
			}
	)";

	// Skybox fragment shader
	std::string kl::skybox::fragSource = R"(
			#version 330

			// Skybox texture
			uniform samplerCube skybox;

			// Interpolated values input
			in vec3 interTex;

			// Pixel color output
			out vec4 pixelColor;

			void main () {
				// Setting the pixel color
				pixelColor = texture(skybox, interTex);
			}
	)";

	// Skybox box vertices
	std::vector<kl::vertex> kl::skybox::boxVertices = {
		kl::vertex(kl::vec3( 1,  1, -1)), kl::vertex(kl::vec3( 1, -1, -1)), kl::vertex(kl::vec3( 1, -1,  1)),
		kl::vertex(kl::vec3( 1, -1,  1)), kl::vertex(kl::vec3( 1,  1,  1)), kl::vertex(kl::vec3( 1,  1, -1)),
		kl::vertex(kl::vec3(-1,  1, -1)), kl::vertex(kl::vec3(-1,  1,  1)), kl::vertex(kl::vec3(-1, -1,  1)),
		kl::vertex(kl::vec3(-1, -1,  1)), kl::vertex(kl::vec3(-1, -1, -1)), kl::vertex(kl::vec3(-1,  1, -1)),
		kl::vertex(kl::vec3( 1,  1, -1)), kl::vertex(kl::vec3( 1,  1,  1)), kl::vertex(kl::vec3(-1,  1,  1)),
		kl::vertex(kl::vec3(-1,  1,  1)), kl::vertex(kl::vec3(-1,  1, -1)), kl::vertex(kl::vec3( 1,  1, -1)),
		kl::vertex(kl::vec3( 1, -1, -1)), kl::vertex(kl::vec3(-1, -1, -1)), kl::vertex(kl::vec3(-1, -1,  1)),
		kl::vertex(kl::vec3(-1, -1,  1)), kl::vertex(kl::vec3( 1, -1,  1)), kl::vertex(kl::vec3( 1, -1, -1)),
		kl::vertex(kl::vec3( 1,  1,  1)), kl::vertex(kl::vec3( 1, -1,  1)), kl::vertex(kl::vec3(-1, -1,  1)),
		kl::vertex(kl::vec3(-1, -1,  1)), kl::vertex(kl::vec3(-1,  1,  1)), kl::vertex(kl::vec3( 1,  1,  1)),
		kl::vertex(kl::vec3( 1,  1, -1)), kl::vertex(kl::vec3(-1,  1, -1)), kl::vertex(kl::vec3(-1, -1, -1)),
		kl::vertex(kl::vec3(-1, -1, -1)), kl::vertex(kl::vec3( 1, -1, -1)), kl::vertex(kl::vec3( 1,  1, -1))
	};
}
