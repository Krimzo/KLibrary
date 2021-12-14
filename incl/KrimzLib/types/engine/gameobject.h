#pragma once


namespace kl {
    struct gameobject {
		// Visual
		bool visible;
		kl::texture* texture;

		// Buffers
		std::vector<kl::vertex> vertices;
		std::vector<kl::index> indices;

		// Geometry
		kl::vec3 size;
		kl::vec3 rotation;
		kl::vec3 position;

		// Physics
		bool physics;
		float gravity;
		kl::vec3 velocity;
		kl::vec3 angular;

		// Constructors
		gameobject() {
			visible = true;
			texture = nullptr;
			vertices = {};
			indices = {};
			position = kl::vec3();
			rotation = kl::vec3();
			size = kl::vec3(1, 1, 1);
			physics = false;
			gravity = 1;
			velocity = kl::vec3();
			angular = kl::vec3();
		}
		gameobject(kl::texture* texture) {
			visible = true;
			this->texture = texture;
			vertices = {};
			indices = {};
			position = kl::vec3();
			rotation = kl::vec3();
			size = kl::vec3(1, 1, 1);
			physics = false;
			gravity = 1;
			velocity = kl::vec3();
			angular = kl::vec3();
		}

		// Returns the pointer to raw vertex data
		void* vertexData() {
			return &vertices[0];
		}

		// Returns the byte size of the vertex data array
		kl::uint64 vertexDataSize() {
			return vertices.size() * sizeof(kl::vertex);
		}

		// Returns the pointer to raw index data
		kl::index* indexData() {
			return &indices[0];
		}

		// Returns the byte size of the index data array
		int indexDataSize() {
			return indices.size() * sizeof(kl::index);
		}

		// Returns the world transformation matrix
		kl::mat4 worldMatrix() {
			return kl::mat4::translate(position) * kl::mat4::rotate(rotation) * kl::mat4::scale(size);
		}
	};
}