#include "gpu/gpu.h"

#include "utility/file.h"


kl::dx::mesh kl::gpu::newVertexBuffer(const std::vector<kl::vertex>& vertexData)
{
	kl::dx::desc::buffer bufferDesc = {};
	bufferDesc.ByteWidth = uint(vertexData.size() * sizeof(kl::vertex));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	kl::dx::desc::subres bufferData = {};
	bufferData.pSysMem = &vertexData[0];

	return newBuffer(&bufferDesc, &bufferData);
}
kl::dx::mesh kl::gpu::newVertexBuffer(const std::string& filePath, bool flipZ)
{
	return newVertexBuffer(kl::file::parseMesh(filePath, flipZ));
}

void kl::gpu::draw(kl::dx::mesh mesh)
{
	const uint stride = sizeof(kl::vertex); uint offset = 0;
	m_Context->IASetVertexBuffers(0, 1, &mesh, &stride, &offset);

	kl::dx::desc::buffer buffDesc = {};
	mesh->GetDesc(&buffDesc);

	m_Context->Draw(buffDesc.ByteWidth / stride, 0);
}
