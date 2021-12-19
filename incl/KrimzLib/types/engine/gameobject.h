#pragma once


namespace kl {
    struct gameobject {
		// Visual
		bool visible;

		// Geometry
		kl::vec3 size;
		kl::vec3 rotation;
		kl::vec3 position;

		// Physics
		bool physics;
		float gravity;
		kl::vec3 velocity;
		kl::vec3 angular;

		// Constructors/destructor
		gameobject() {
			visible = true;
			position = kl::vec3();
			rotation = kl::vec3();
			size = kl::vec3(1, 1, 1);
			physics = false;
			gravity = 1;
			velocity = kl::vec3();
			angular = kl::vec3();
			indexCount = 0;
			VBO = nullptr;
			IBO = nullptr;
			VAO = nullptr;
			shaders = nullptr;
			texture = nullptr;
		}
		~gameobject() {
			if (VBO) {
				delete VBO;
			}
			if (IBO) {
				delete IBO;
			}
			if (VAO) {
				delete VAO;
			}
		}

		// Sets the vertex data
		void setData(std::vector<kl::vertex>& vertexData, std::vector<kl::index>& indexData) {
			// Checking/creating the vao
			if (!VAO) VAO = new kl::vao();

			// Defining the vao
			VAO->define([&]() {
				// Setting up the vbo
				VBO = new kl::vbo();
				VBO->setData(&vertexData[0], vertexData.size() * sizeof(kl::vertex), GL_STATIC_DRAW);
				VBO->setParsing(0, GL_FLOAT, 3, sizeof(kl::vertex), 0);
				VBO->setParsing(1, GL_FLOAT, 2, sizeof(kl::vertex), sizeof(kl::vec3));
				VBO->setParsing(2, GL_FLOAT, 3, sizeof(kl::vertex), sizeof(kl::vec3) + sizeof(kl::vec2));

				// Setting up the ibo
				IBO = new kl::ibo();
				IBO->setData(&indexData[0], indexData.size() * sizeof(kl::index), GL_STATIC_DRAW);
				indexCount = indexData.size();
			});
		}

		// Shader getter
		kl::shaders* getShaders() {
			return shaders;
		}

		// Shader setter
		void setShaders(kl::shaders* shaders) {
			this->shaders = shaders;
		}

		// Texture getter
		kl::texture* getTexture() {
			return texture;
		}

		// Texture setter
		void setTexture(kl::texture* texture) {
			this->texture = texture;
		}

		// Render the game object
		void render() {
			shaders->use();
			texture->bind();
			VAO->drawElements(GL_TRIANGLES, indexCount);
		}

		// Returns the world transformation matrix
		kl::mat4 matrix() {
			return kl::mat4::translate(position) * kl::mat4::rotate(rotation) * kl::mat4::scale(size);
		}

	private:
		// Buffers
		kl::uint32 indexCount;
		kl::vbo* VBO;
		kl::ibo* IBO;
		kl::vao* VAO;
		kl::shaders* shaders;
		kl::texture* texture;
	};
}