#include "gpu/gpu.h"

#include "utility/file.h"


// Constructors
ID3D11Buffer* kl::gpu::newVertBuffer(const std::vector<kl::vertex>& vertexData) {
	// Buffer descriptor creation
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = UINT(vertexData.size() * sizeof(kl::vertex));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	// Buffer data descriptor creation
	D3D11_SUBRESOURCE_DATA bufferData = {};
	bufferData.pSysMem = &vertexData[0];

	// Return
	return this->newBuffer(&bufferDesc, &bufferData);
}
ID3D11Buffer* kl::gpu::newVertBuffer(const String& filePath, bool flipZ) {
	return this->newVertBuffer(kl::file::parseObj(filePath, flipZ));
}

// Renders the mesh
void kl::gpu::draw(ID3D11Buffer* mesh) {
	// Binding the mesh
	const UINT stride = sizeof(kl::vertex); UINT offset = 0;
	devcon->IASetVertexBuffers(0, 1, &mesh, &stride, &offset);

	// Getting the byte size
	D3D11_BUFFER_DESC buffDesc = {};
	mesh->GetDesc(&buffDesc);

	// Drawing
	devcon->Draw(buffDesc.ByteWidth / stride, 0);
}
