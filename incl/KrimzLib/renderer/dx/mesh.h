#pragma once


namespace kl {
	namespace dx {
		struct mesh {
			// Constructors/destructor
			mesh(ID3D11Device* dev, std::vector<kl::vertex>& vertexData) {
				loadData(dev, vertexData);
			}
			mesh(ID3D11Device* dev, std::string filePath, bool flipZ) {
				loadFromFile(dev, filePath, flipZ);
			}
			~mesh() {
				buff->Release();
			}

			// Buffer getter
			ID3D11Buffer* getBuff() {
				return buff;
			}

			// Returns the vertex count
			int vertCount() {
				return vertexCount;
			}

		private:
			ID3D11Buffer* buff = nullptr;
			int vertexCount = 0;

			// Sets the vertex/index data
			void loadData(ID3D11Device* dev, std::vector<kl::vertex>& vertexData) {
				// Saving the vertex count
				vertexCount = vertexData.size();

				// Buffer descriptor setting
				D3D11_BUFFER_DESC bufferDescriptor = {};
				bufferDescriptor.ByteWidth = sizeof(kl::vertex) * vertexData.size();
				bufferDescriptor.Usage = D3D11_USAGE_DEFAULT;
				bufferDescriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;
				bufferDescriptor.CPUAccessFlags = NULL;
				bufferDescriptor.MiscFlags = NULL;
				bufferDescriptor.StructureByteStride = 0;

				// Buffer data setting
				D3D11_SUBRESOURCE_DATA bufferData = {};
				bufferData.pSysMem = &vertexData[0];
				bufferData.SysMemPitch = 0;
				bufferData.SysMemSlicePitch = 0;

				// Buffer creation
				dev->CreateBuffer(&bufferDescriptor, &bufferData, &buff);
				if (!buff) kl::console::error("DirectX: Could not create a vertex buffer!");
			}

			// Loads the vertex data from a .obj file 
			void loadFromFile(ID3D11Device* dev, std::string filePath, bool flipZ) {
				// Temp vertex buffer
				std::vector<kl::vertex> vertexData;

				// Opening the file
				std::fstream fileStream;
				fileStream.open(filePath, std::ios::in);
				if (!fileStream.is_open()) kl::console::error("DirectX: Could not open an object file!");

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
				loadData(dev, vertexData);
			}
		};
	}
}
