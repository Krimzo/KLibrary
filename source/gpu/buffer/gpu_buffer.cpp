#include "gpu/gpu.h"

#include "utility/console.h"


ID3D11Buffer* kl::gpu::newBuffer(D3D11_BUFFER_DESC* desc, D3D11_SUBRESOURCE_DATA* subData) {
	ID3D11Buffer* constBuff = nullptr;
	m_Device->CreateBuffer(desc, subData, &constBuff);
	kl::console::error(!constBuff, "Failed to create buffer");

	m_Children.insert(constBuff);
	return constBuff;
}
