#include "gpu/gpu.h"

#include "utility/console.h"


// Constant buffer
ID3D11Buffer* kl::gpu::newConstBuffer(int byteSize) {
	// Checking the size
	if (byteSize % 16 != 0) {
		kl::console::show();
		std::cout << "DirectX: Constant buffer size has to be a multiple of 16!";
		std::cin.get();
		exit(69);
	}

	// Buffer descriptor
	D3D11_BUFFER_DESC buffDesc = {};
	buffDesc.ByteWidth = byteSize;
	buffDesc.Usage = D3D11_USAGE_DYNAMIC;
	buffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	// Return
	return this->newBuffer(&buffDesc);
}

// Sets the buffer data
void kl::gpu::setBuffData(ID3D11Buffer* buff, void* data) {
	// Getting the buffer size
	D3D11_BUFFER_DESC buffData = {};
	buff->GetDesc(&buffData);

	// Mapping and setting buffer data
	D3D11_MAPPED_SUBRESOURCE ms = {};
	devcon->Map(buff, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, data, buffData.ByteWidth);
	devcon->Unmap(buff, NULL);
}

// Binds the buffer to shader
void kl::gpu::bindVertCBuff(ID3D11Buffer* buff, int slot) {
	devcon->VSSetConstantBuffers(slot, 1, &buff);
}
void kl::gpu::bindPixlCBuff(ID3D11Buffer* buff, int slot) {
	devcon->PSSetConstantBuffers(slot, 1, &buff);
}
