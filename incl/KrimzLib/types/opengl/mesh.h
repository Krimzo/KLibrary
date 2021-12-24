#pragma once


namespace kl {
	struct mesh {
		// Constructors/destructor
		mesh(std::vector<kl::vertex>& vertexData) {
			loadData(vertexData);
		}
		mesh(std::string filePath, bool flipZ = true) {
			loadFromFile(filePath, flipZ);
		}
		~mesh() {
			glDeleteVertexArrays(1, &vao);
			glDeleteBuffers(1, &vbo);
		}

		// Renders the mesh
		void draw() {
			// Binding the vao
			glBindVertexArray(vao);

			// Drawing vertices
			glDrawArrays(GL_TRIANGLES, 0, vertexCount);

			// Unbinding the vao
			glBindVertexArray(NULL);
		}

	private:
		kl::id vao = NULL;
		kl::id vbo = NULL;
		int vertexCount = 0;

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
			glEnableVertexAttribArray(2);

			// Setting the data parsing type
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(kl::vertex), (void*)0);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(kl::vertex), (void*)sizeof(kl::vec3));
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(kl::vertex), (void*)(sizeof(kl::vec3) + sizeof(kl::vec2)));

			// Unbinding the vao
			glBindVertexArray(NULL);

			// Disabling the attributes
			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
			glDisableVertexAttribArray(2);

			// Unbinding the vbo
			glBindBuffer(GL_ARRAY_BUFFER, NULL);
		}

		// Loads the vertex data from a .obj file 
		void loadFromFile(std::string filePath, bool flipZ) {
			// Temp vertex buffer
			std::vector<kl::vertex> vertexData;

			// Opening the file
			std::fstream fileStream;
			fileStream.open(filePath, std::ios::in);
			if (!fileStream.is_open()) exit(69);

			// Temp load buffers
			std::vector<kl::vec3> xyzBuffer;
			std::vector<kl::vec2> uvBuffer;
			std::vector<kl::vec3> normBuffer;

			// Z flipper
			const int zFlip = flipZ ? -1 : 1;

			// Parsing data
			std::string fileLine;
			while (std::getline(fileStream, fileLine)) {
				// Splitting the string by spaces
				std::vector<std::string> lineParts;
				std::stringstream lineStream(fileLine);
				for (std::string linePart; std::getline(lineStream, linePart, ' ');) {
					lineParts.push_back(linePart);
				}

				// Parsing the data
				if (lineParts[0] == "v") {
					xyzBuffer.push_back(kl::vec3(std::stof(lineParts[1]), std::stof(lineParts[2]), zFlip * std::stof(lineParts[3])));
				}
				else if (lineParts[0] == "vt") {
					uvBuffer.push_back(kl::vec2(std::stof(lineParts[1]), std::stof(lineParts[2])));
				}
				else if (lineParts[0] == "vn") {
					normBuffer.push_back(kl::vec3(std::stof(lineParts[1]), std::stof(lineParts[2]), zFlip * std::stof(lineParts[3])));
				}
				else if (lineParts[0] == "f") {
					for (int i = 1; i < 4; i++) {
						// Getting the world, texture and normal indexes
						std::vector<std::string> linePartParts;
						std::stringstream linePartStream(lineParts[i]);
						for (std::string linePartPart; std::getline(linePartStream, linePartPart, '/');) {
							linePartParts.push_back(linePartPart);
						}

						// Saving the data
						vertexData.push_back(
							kl::vertex(
								xyzBuffer[std::stoi(linePartParts[0]) - 1],
								uvBuffer[std::stoi(linePartParts[1]) - 1],
								normBuffer[std::stoi(linePartParts[2]) - 1]
							)
						);
					}
				}
			}

			// Closing the file
			fileStream.close();

			// Loading the data
			loadData(vertexData);
		}
	};
}
