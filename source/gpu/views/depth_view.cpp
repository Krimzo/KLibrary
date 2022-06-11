#include "gpu/gpu.h"

#include "utility/console.h"


ID3D11DepthStencilView* kl::gpu::newDepthView(ID3D11Texture2D* tex, D3D11_DEPTH_STENCIL_VIEW_DESC* desc) {
	ID3D11DepthStencilView* depthView = nullptr;
	m_Device->CreateDepthStencilView(tex, desc, &depthView);
	kl::console::error(!depthView, "Failed to create depth stencil view");

	m_Children.insert(depthView);
	return depthView;
}

void kl::gpu::clear(ID3D11DepthStencilView* view, float depth, byte stencil) {
	m_Context->ClearDepthStencilView(view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, stencil);
}
