#include "GPU/GPU.h"

#include "Utility/File.h"


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

kl::dx::Buffer kl::GPU::generateScreenMesh() {
	return newVertexBuffer({
		{ { -1.0f, -1.0f, 0.5f } }, { { -1.0f, 1.0f, 0.5f } }, { { 1.0f, 1.0f, 0.5f } },
		{ { 1.0f, 1.0f, 0.5f } }, { { 1.0f, -1.0f, 0.5f } }, { { -1.0f, -1.0f, 0.5f } }
	});
}

kl::dx::Buffer kl::GPU::generatePlaneMesh(float size, int numOfPoints) {
	// Count fix
	float increment = size / numOfPoints;
	numOfPoints += 1;

	// Generating points
	Vector<Float2> points;
	points.reserve(uint64(numOfPoints) * numOfPoints);
	for (int x = 0; x < numOfPoints; x++) {
		for (int z = 0; z < numOfPoints; z++) {
			points.push_back({
				-size * 0.5f + x * increment,
				-size * 0.5f + z * increment
			});
		}
	}

	// Generating triangles
	Vector<Vertex> vertices;
	vertices.reserve(uint64(numOfPoints - 1) * uint64(numOfPoints - 1) * 6);
	for (int x = 0; x < numOfPoints - 1; x++) {
		for (int z = 0; z < numOfPoints - 1; z++) {
			Float2 xzPoint = points[uint64(z) * numOfPoints + x];
			Float2 xzPlusPoint = points[uint64(z + 1) * numOfPoints + uint64(x + 1)];

			vertices.push_back({ { xzPlusPoint.x, 0, xzPlusPoint.y } });
			vertices.push_back({ { xzPlusPoint.x, 0, xzPoint.y } });
			vertices.push_back({ { xzPoint.x, 0, xzPoint.y } });

			vertices.push_back({ { xzPoint.x, 0, xzPoint.y } });
			vertices.push_back({ { xzPoint.x, 0, xzPlusPoint.y } });
			vertices.push_back({ { xzPlusPoint.x, 0, xzPlusPoint.y } });
		}
	}
	return newVertexBuffer(vertices);
}

void kl::GPU::drawVertexBuffer(dx::Buffer buffer) {
	constexpr uint stride = sizeof(Vertex); constexpr uint offset = 0;
	m_Context->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);

	dx::BufferDesc descriptor = {};
	buffer->GetDesc(&descriptor);

	m_Context->Draw(descriptor.ByteWidth / stride, 0);
}
