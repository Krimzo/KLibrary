#include "KrimzLib/dx/mesh.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "KrimzLib/geometry/vertex.h"
#include "KrimzLib/utility/file.h"


// Constructors
kl::mesh::mesh(ID3D11Device* dev, ID3D11DeviceContext* devcon, const std::vector<kl::vertex>& vertexData) {
	// Saving the devcon
	this->devcon = devcon;

    // Saving the vertices
    this->vertexCount = UINT(vertexData.size());

    // Buffer descriptor creation
    D3D11_BUFFER_DESC bufferDescriptor = {};
    bufferDescriptor.ByteWidth = UINT(sizeof(kl::vertex) * vertexData.size());
    bufferDescriptor.Usage = D3D11_USAGE_DEFAULT;
    bufferDescriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDescriptor.CPUAccessFlags = NULL;
    bufferDescriptor.MiscFlags = NULL;
    bufferDescriptor.StructureByteStride = 0;

    // Buffer data descriptor creation
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
kl::mesh::mesh(ID3D11Device* dev, ID3D11DeviceContext* devcon, const std::string& filePath, bool flipZ) {
    this->mesh::mesh(dev, devcon, kl::file::parseObj(filePath, flipZ));
}

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
	devcon->Draw(vertexCount, 0);
}
