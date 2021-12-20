#pragma once


namespace kl {
	struct mesh {
		// Constructors/destructor
		mesh(std::vector<kl::vertex>& vertexData) {
			loadData(vertexData);
		}
		mesh(std::string filePath) {
			loadFromFile(filePath);
		}
		~mesh() {
			glDeleteVertexArrays(1, &vao);
			glDeleteBuffers(1, &vbo);
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

		// Sets the vertex/index data
		void loadData(std::vector<kl::vertex>& vertexData) {
			// Saving the vertex count
			vertexCount = vertexData.size();

			// Generating the vao
			glGenVertexArrays(1, &vao);

			// Binding the vao
			glBindVertexArray(vao);

			// Generating the vbo
			glGenBuffers(1, &vbo);

			// Binding the vbo
			glBindBuffer(GL_ARRAY_BUFFER, vbo);

			// Sending the data to the vbo
			glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(kl::vertex), &vertexData[0], GL_STATIC_DRAW);

			// Enabling the attributes
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);

			// Setting the data parsing type
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(kl::vertex), (void*)0);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(kl::vertex), (void*)sizeof(kl::vec3));

			// Unbinding the vao
			glBindVertexArray(NULL);

			// Disabling the attributes
			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);

			// Unbinding the vbo
			glBindBuffer(GL_ARRAY_BUFFER, NULL);
		}

		// Loads the vertex data from a .obj file 
		void loadFromFile(std::string filePath) {
			// Temp vertex buffer
			std::vector<kl::vertex> vertexData;

			// File opening
			FILE* fileStream = fopen(filePath.c_str(), "r");
			if (!fileStream) exit(69);

			// Temp load buffers
			std::vector<kl::vec3> xyzBuffer;
			std::vector<kl::vec2> uvBuffer;

			// Parsing data
			int scanStatus = 0;
			while (scanStatus != -1) {
				// Temp buffers
				int wi0, ti0, ni0, wi1, ti1, ni1, wi2, ti2, ni2;
				kl::vec3 tempXYZ;
				kl::vec2 tempUV;
				char trashBuff[256];

				// Reading data
				if ((scanStatus = fscanf(fileStream, "v %f %f %f", &tempXYZ.x, &tempXYZ.y, &tempXYZ.z)) == 3) {
					xyzBuffer.push_back(tempXYZ);
				}
				else if ((scanStatus = fscanf(fileStream, "t %f %f", &tempUV.x, &tempUV.y)) == 2) {
					uvBuffer.push_back(tempUV);
				}
				else if ((scanStatus = fscanf(fileStream, "f %d/%d/%d %d/%d/%d %d/%d/%d", &wi0, &ti0, &ni0, &wi1, &ti1, &ni1, &wi2, &ti2, &ni2)) == 9) {
					vertexData.push_back(kl::vertex(xyzBuffer[--wi0], uvBuffer[--ti0]));
					vertexData.push_back(kl::vertex(xyzBuffer[--wi1], uvBuffer[--ti1]));
					vertexData.push_back(kl::vertex(xyzBuffer[--wi2], uvBuffer[--ti2]));
				}
				else {
					fgets(trashBuff, 256, fileStream);
				}
			}
			fclose(fileStream);

			// Sending the data
			loadData(vertexData);
		}
	};
}
