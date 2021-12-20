#pragma once


namespace kl {
    struct gameobject {
		// Object data
		bool visible = true;
		kl::geometry geometry = kl::geometry();
		kl::physics physics = kl::physics();
		
		// Loads the vertex data to a mesh
		void loadData(std::vector<kl::vertex>& vertexData) {
			mesh.loadData(vertexData);
		}
		void loadData(std::string filePath) {
			mesh.loadData(filePath);
		}

		// Sets the object texture pointer
		void setTexture(kl::texture* texture) {
			this->texture = texture;
		}

		// Renders the game object
		void render() {
			texture->bind();
			mesh.drawArrays();
		}

	private:
		// Object data
		kl::mesh mesh = kl::mesh();
		kl::texture* texture = nullptr;
	};
}