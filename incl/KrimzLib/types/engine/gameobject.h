#pragma once


namespace kl {
    struct gameobject {
		bool visible = true;
		kl::geometry geometry = kl::geometry();
		kl::physics physics = kl::physics();

		// Vertex/index data setter
		void setData(std::vector<kl::vertex>& vertexData) {
			mesh.setData(vertexData);
		}
		void setData(std::vector<kl::vertex>& vertexData, std::vector<kl::index>& indexData) {
			mesh.setData(vertexData, indexData);
		}

		// Loads vertex/index data from .obj file
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

			// Parse data
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
			setData(vertexData);
		}

		// Texture setter
		void setTexture(kl::texture* texture) {
			this->texture = texture;
		}

		// Render the game object
		void drawArrays() {
			texture->bind();
			mesh.drawArrays();
		}
		void drawElements() {
			texture->bind();
			mesh.drawElements();
		}

	private:
		kl::mesh mesh = kl::mesh();
		kl::texture* texture = nullptr;
	};
}