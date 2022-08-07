#include "GPU/GPU.h"

#include "Utility/Console.h"


kl::dx::Buffer kl::GPU::newCBuffer(uint byteSize) {
	Assert(byteSize % 16, "Constant buffer size has to be a multiple of 16");

	dx::BufferDesc descriptor = {};
	descriptor.ByteWidth = byteSize;
	descriptor.Usage = D3D11_USAGE_DYNAMIC;
	descriptor.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	descriptor.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	return newBuffer(&descriptor);
}

void kl::GPU::setCBufferData(dx::Buffer buffer, const void* data) {
	dx::BufferDesc descriptor = {};
	buffer->GetDesc(&descriptor);

	D3D11_MAPPED_SUBRESOURCE mapped = {};
	m_Context->Map(buffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mapped);
	memcpy(mapped.pData, data, descriptor.ByteWidth);
	m_Context->Unmap(buffer, NULL);
}

void kl::GPU::bindVertexCBuffer(dx::Buffer buffer, uint slot) {
	m_Context->VSSetConstantBuffers(slot, 1, &buffer);
}

void kl::GPU::bindPixelCBuffer(dx::Buffer buffer, uint slot) {
	m_Context->PSSetConstantBuffers(slot, 1, &buffer);
}

void kl::GPU::bindComputeCBuffer(dx::Buffer buffer, uint slot) {
	m_Context->CSSetConstantBuffers(slot, 1, &buffer);
}
