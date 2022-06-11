#include "gpu/gpu.h"

#include "utility/console.h"


ID3D11Buffer* kl::gpu::newCBuffer(uint byteSize) {
	kl::console::error(byteSize % 16, "Constant buffer size has to be a multiple of 16");

	D3D11_BUFFER_DESC buffDesc = {};
	buffDesc.ByteWidth = byteSize;
	buffDesc.Usage = D3D11_USAGE_DYNAMIC;
	buffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	return this->newBuffer(&buffDesc);
}

void kl::gpu::setCBufferData(ID3D11Buffer* buff, const void* data) {
	D3D11_BUFFER_DESC buffData = {};
	buff->GetDesc(&buffData);

	D3D11_MAPPED_SUBRESOURCE ms = {};
	m_Context->Map(buff, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, data, buffData.ByteWidth);
	m_Context->Unmap(buff, NULL);
}

void kl::gpu::bindVertexCBuffer(ID3D11Buffer* buff, uint slot) {
	m_Context->VSSetConstantBuffers(slot, 1, &buff);
}
void kl::gpu::bindPixelCBuffer(ID3D11Buffer* buff, uint slot) {
	m_Context->PSSetConstantBuffers(slot, 1, &buff);
}
