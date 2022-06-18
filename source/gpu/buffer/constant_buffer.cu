#include "gpu/gpu.cuh"

#include "utility/console.cuh"


kl::dx::buffer kl::gpu::newCBuffer(uint byteSize) {
	kl::console::error(byteSize % 16, "Constant buffer size has to be a multiple of 16");

	kl::dx::desc::buffer buffDesc = {};
	buffDesc.ByteWidth = byteSize;
	buffDesc.Usage = D3D11_USAGE_DYNAMIC;
	buffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	return newBuffer(&buffDesc);
}

void kl::gpu::setCBufferData(kl::dx::buffer m_Buffer, const void* data) {
	kl::dx::desc::buffer buffData = {};
	m_Buffer->GetDesc(&buffData);

	D3D11_MAPPED_SUBRESOURCE ms = {};
	m_Context->Map(m_Buffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, data, buffData.ByteWidth);
	m_Context->Unmap(m_Buffer, NULL);
}

void kl::gpu::bindVertexCBuffer(kl::dx::buffer m_Buffer, uint slot) {
	m_Context->VSSetConstantBuffers(slot, 1, &m_Buffer);
}
void kl::gpu::bindPixelCBuffer(kl::dx::buffer m_Buffer, uint slot) {
	m_Context->PSSetConstantBuffers(slot, 1, &m_Buffer);
}
