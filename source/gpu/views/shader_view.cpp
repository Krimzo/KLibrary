#include "gpu/gpu.h"

#include "utility/console.h"


ID3D11ShaderResourceView* kl::gpu::newShaderView(ID3D11Texture2D* tex, D3D11_SHADER_RESOURCE_VIEW_DESC* desc) {
	ID3D11ShaderResourceView* shaderView = nullptr;
	m_Device->CreateShaderResourceView(tex, desc, &shaderView);
	kl::console::error(!shaderView, "Failed to create shader view");

	m_Children.insert(shaderView);
	return shaderView;
}

void kl::gpu::bindPixelShaderView(ID3D11ShaderResourceView* view, uint slot) {
	m_Context->PSSetShaderResources(slot, 1, &view);
}
void kl::gpu::bindComputeShaderView(ID3D11ShaderResourceView* view, uint slot) {
	m_Context->CSSetShaderResources(slot, 1, &view);
}
