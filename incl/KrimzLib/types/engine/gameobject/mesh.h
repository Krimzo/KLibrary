#pragma once


namespace kl {
	struct mesh {
		// Buffers
		kl::uint32 indexCount = 0;
		kl::vbo VBO = kl::vbo();
		kl::ibo IBO = kl::ibo();
		kl::vao VAO = kl::vao();

		// Sets the vertex data
		void setData(std::vector<kl::vertex>& vertexData, std::vector<kl::index>& indexData) {
			// Setting up the vao
			VAO.define([&]() {
				// Setting up the vbo
				VBO.setData(&vertexData[0], vertexData.size() * sizeof(kl::vertex), GL_STATIC_DRAW);
				VBO.setParsing(0, GL_FLOAT, 3, sizeof(kl::vertex), 0);
				VBO.setParsing(1, GL_FLOAT, 2, sizeof(kl::vertex), sizeof(kl::vec3));
				VBO.setParsing(2, GL_FLOAT, 3, sizeof(kl::vertex), sizeof(kl::vec3) + sizeof(kl::vec2));

				// Setting up the ibo
				IBO.setData(&indexData[0], indexData.size() * sizeof(kl::index), GL_STATIC_DRAW);
				indexCount = indexData.size();
			});
		}

		// Renders the mesh
		void render() {
			VAO.drawElements(GL_TRIANGLES, indexCount);
		}
	};
}
