#include "gpu/gpu.h"

#include "utility/console.h"


ID3D11ShaderResourceView* kl::gpu::newShaderView(ID3D11Texture2D* tex, D3D11_SHADER_RESOURCE_VIEW_DESC* desc) {
	// Creating the render target view
	ID3D11ShaderResourceView* shaderView = nullptr;
	m_Device->CreateShaderResourceView(tex, desc, &shaderView);
	if (!shaderView) {
		kl::console::show();
		std::cout << "DirectX: Could not create a backbuffer!";
		std::cin.get();
		exit(69);
	}

	// Saving child
	m_Children.insert(shaderView);

	// Return
	return shaderView;
}

// Binds the view
void kl::gpu::bindPixelShaderView(ID3D11ShaderResourceView* view, uint slot) {
	m_Context->PSSetShaderResources(slot, 1, &view);
}
void kl::gpu::bindComputeShaderView(ID3D11ShaderResourceView* view, uint slot) {
	m_Context->CSSetShaderResources(slot, 1, &view);
}
