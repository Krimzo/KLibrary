#include "gpu/gpu.h"

#include "utility/file.h"


kl::dx::Buffer kl::GPU::newVertexBuffer(const Vector<Vertex>& vertexData) {
	dx::BufferDesc descriptor = {};
	descriptor.ByteWidth = uint(vertexData.size() * sizeof(Vertex));
	descriptor.Usage = D3D11_USAGE_DEFAULT;
	descriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	dx::SubresDesc data = {};
	data.pSysMem = vertexData.data();

	return newBuffer(&descriptor, &data);
}

kl::dx::Buffer kl::GPU::newVertexBuffer(const String& filePath, bool flipZ) {
	return newVertexBuffer(Files::ParseMesh(filePath, flipZ));
}

void kl::GPU::drawVertexBuffer(dx::Buffer buffer) {
	constexpr uint stride = sizeof(Vertex); constexpr uint offset = 0;
	m_Context->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);

	dx::BufferDesc descriptor = {};
	buffer->GetDesc(&descriptor);

	m_Context->Draw(descriptor.ByteWidth / stride, 0);
}
