#pragma once


namespace kl {
	struct mesh {
		// Destructor
		~mesh() {
			if (vao) {
				glDeleteVertexArrays(1, &vao);
			}
			if (vbo) {
				glDeleteBuffers(1, &vbo);
			}
		}

		// Sets the vertex/index data
		void loadData(std::vector<kl::vertex>& vertexData) {
			// Saving the vertex count
			vertexCount = vertexData.size();

			// Checking/generating the vao
			if (!vao) {
				glGenVertexArrays(1, &vao);
			}

			// Binding the vao
			glBindVertexArray(vao);

			// Checking/generating the vbo
			if (!vbo) {
				glGenBuffers(1, &vbo);
			}

			// Binding the vbo
			glBindBuffer(GL_ARRAY_BUFFER, vbo);

			// Sending the data to the vbo
			glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(kl::vertex), &vertexData[0], GL_STATIC_DRAW);

			// Setting up the data parsing
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(kl::vertex), (void*)0);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(kl::vertex), (void*)sizeof(kl::vec3));

			// Unbinding the vao
			glBindVertexArray(NULL);

			// Unbinding the vbo
			glBindBuffer(GL_ARRAY_BUFFER, NULL);
		}
		void loadData(std::string filePath) {
			// Temp vertex buffer
			std::vector<kl::vertex> vertexData;

			// File loading
			FILE* fileStream = fopen(filePath.c_str(), "r");
			if (!fileStream) exit(69);

			// Data buffers
			std::vector<kl::vec3> xyzBuffer;
			std::vector<kl::vec2> uvBuffer;
			char tempBuffer[256];
			int scanStatus = 0;
			kl::vec3 tempXYZ = {};
			kl::vec2 tempUV = {};
			int coordIndex0 = 0;
			int textureIndex0 = 0;
			int normalIndx0 = 0;
			int coordIndex1 = 0;
			int textureIndex1 = 0;
			int normalIndx1 = 0;
			int coordIndex2 = 0;
			int textureIndex2 = 0;
			int normalIndx2 = 0;

			// Parsing data
			while (scanStatus != -1) {
				if ((scanStatus = fscanf(fileStream, "v %f %f %f", &tempXYZ.x, &tempXYZ.y, &tempXYZ.z)) == 3) {
					xyzBuffer.push_back(tempXYZ);
				}
				else if ((scanStatus = fscanf(fileStream, "t %f %f", &tempUV.x, &tempUV.y)) == 2) {
					uvBuffer.push_back(tempUV);
				}
				else if ((scanStatus = fscanf(fileStream, "f %d/%d/%d %d/%d/%d %d/%d/%d", &coordIndex0, &textureIndex0, &normalIndx0, &coordIndex1, &textureIndex1, &normalIndx1, &coordIndex2, &textureIndex2, &normalIndx2)) == 9) {
					vertexData.push_back(kl::vertex(xyzBuffer[--coordIndex0], uvBuffer[--textureIndex0]));
					vertexData.push_back(kl::vertex(xyzBuffer[--coordIndex1], uvBuffer[--textureIndex1]));
					vertexData.push_back(kl::vertex(xyzBuffer[--coordIndex2], uvBuffer[--textureIndex2]));
				}
				else {
					fgets(tempBuffer, 256, fileStream);
				}
			}
			fclose(fileStream);

			// Sending the data
			loadData(vertexData);
		}

		// Renders the mesh
		void drawArrays() {
			// Binding the vao
			glBindVertexArray(vao);

			// Rendering
			glDrawArrays(GL_TRIANGLES, 0, vertexCount);

			// Unbinding the vao
			glBindVertexArray(NULL);
		}

	private:
		kl::id vao = NULL;
		kl::id vbo = NULL;
		kl::uint32 vertexCount = 0;
	};
}
