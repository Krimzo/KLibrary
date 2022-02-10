#include "KrimzLib/dx/mesh.h"

#include "KrimzLib/renderer/vertex.h"


// Constructors
kl::mesh::mesh(ID3D11DeviceContext* devcon, ID3D11Buffer* buff, int vertexCount) {
	this->devcon = devcon;
	this->buff = buff;
	this->vertexCount = vertexCount;
}

// Destructor
kl::mesh::~mesh() {
	if (buff) buff->Release();
}

// Renders the mesh
void kl::mesh::draw() const {
	// Binding the mesh
	UINT tempStride = sizeof(kl::vertex);
	UINT tempOffset = 0;
	ID3D11Buffer* tempBuff = this->buff;
	devcon->IASetVertexBuffers(0, 1, &tempBuff, &tempStride, &tempOffset);

	// Drawing
	devcon->Draw(vertexCount, 0);
}
