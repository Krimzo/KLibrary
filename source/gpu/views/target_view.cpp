#include "gpu/gpu.h"

#include "utility/console.h"


ID3D11RenderTargetView* kl::gpu::newTargetView(ID3D11Texture2D* tex, D3D11_RENDER_TARGET_VIEW_DESC* desc) {
	// Creating the render target view
	ID3D11RenderTargetView* targetView = nullptr;
	m_Device->CreateRenderTargetView(tex, desc, &targetView);
	if (!targetView) {
		kl::console::show();
		std::cout << "DirectX: Could not create a backbuffer!";
		std::cin.get();
		exit(69);
	}

	// Saving child
	m_Children.insert(targetView);

	// Return
	return targetView;
}

void kl::gpu::clear(ID3D11RenderTargetView* view, const kl::float4& color) {
	m_Context->ClearRenderTargetView(view, (float*)&color);
}
