#include "gpu/gpu.h"

#include "utility/console.h"


ID3D11ShaderResourceView* kl::gpu::newShaderView(ID3D11Texture2D* tex, D3D11_SHADER_RESOURCE_VIEW_DESC* desc) {
	// Creating the render target view
	ID3D11ShaderResourceView* shaderView = nullptr;
	device->CreateShaderResourceView(tex, desc, &shaderView);
	if (!shaderView) {
		kl::console::show();
		std::cout << "DirectX: Could not create a backbuffer!";
		std::cin.get();
		exit(69);
	}

	// Saving child
	children.insert(shaderView);

	// Return
	return shaderView;
}

// Binds the texture
void kl::gpu::bindPixlTex(ID3D11ShaderResourceView* buff, uint32_t slot) {
	devcon->PSSetShaderResources(slot, 1, &buff);
}
