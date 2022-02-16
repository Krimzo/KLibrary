#include "KrimzLib/dx/mesh.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "KrimzLib/geometry/vertex.h"


// Constructors
kl::mesh::mesh(ID3D11Device* dev, ID3D11DeviceContext* devcon, const std::vector<kl::vertex>& vertexData) {
	// Saving the devcon
	this->devcon = devcon;

    // Saving the vertices
    this->vertices = vertexData;

    // Buffer descriptor creation
    D3D11_BUFFER_DESC bufferDescriptor = {};
    bufferDescriptor.ByteWidth = UINT(sizeof(kl::vertex) * vertexData.size());
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
    if (!buff) {
        std::cout << "DirectX: Could not create a vertex buffer!";
        std::cin.get();
        exit(69);
    }
}
kl::mesh::mesh(ID3D11Device* dev, ID3D11DeviceContext* devcon, const std::string& filePath, bool flipZ) : kl::mesh(dev, devcon, kl::mesh::parseFile(filePath, flipZ)) {}

// Destructor
kl::mesh::~mesh() {
	buff->Release();
}

// Renders the mesh
void kl::mesh::draw() const {
	// Binding the mesh
	const UINT tempStride = sizeof(kl::vertex);
	const UINT tempOffset = 0;
	devcon->IASetVertexBuffers(0, 1, &buff, &tempStride, &tempOffset);

	// Drawing
	devcon->Draw(UINT(vertices.size()), 0);
}

// Parses .obj file
std::vector<kl::vertex> kl::mesh::parseFile(const std::string& filePath, bool flipZ) {
    // Temp vertex buffer
    std::vector<kl::vertex> vertexData;

    // Opening the file
    std::fstream fileStream;
    fileStream.open(filePath, std::ios::in);
    if (!fileStream.is_open()) {
        std::cout << "Mesh: Could not open an object file!\nFile: " << filePath;
        std::cin.get();
        exit(69);
    }

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

    // Returning
    return vertexData;
}
