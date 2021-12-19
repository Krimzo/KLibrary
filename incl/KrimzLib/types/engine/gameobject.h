#pragma once


namespace kl {
    struct gameobject {
		bool visible = true;
		kl::geometry geometry = kl::geometry();
		kl::physics physics = kl::physics();

		// Vertex/index data setter
		void setData(std::vector<kl::vertex>& vertexData, std::vector<kl::index>& indexData) {
			mesh.setData(vertexData, indexData);
		}

		// Shader setter
		void setShaders(kl::shaders* shaders, std::string wvpName) {
			this->shaders = shaders;
			wvp = shaders->getUniform(wvpName);
		}

		// Texture setter
		void setTexture(kl::texture* texture) {
			this->texture = texture;
		}

		// Render the game object
		void render(kl::mat4& cameraMat) {
			shaders->setUniform(wvp, cameraMat * geometry.matrix());
			texture->bind();
			mesh.render();
		}

	private:
		kl::mesh mesh = kl::mesh();
		kl::shaders* shaders = nullptr;
		kl::texture* texture = nullptr;
		kl::id wvp = NULL;
	};
}