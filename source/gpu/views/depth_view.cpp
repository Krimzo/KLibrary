#include "gpu/gpu.h"

#include "utility/console.h"


kl::dx::view::depth kl::gpu::newDepthView(kl::dx::texture tex, kl::dx::view::desc::depth* desc) {
	kl::dx::view::depth depthView = nullptr;
	m_Device->CreateDepthStencilView(tex, desc, &depthView);
	kl::console::error(!depthView, "Failed to create depth stencil view");

	m_Children.insert(depthView);
	return depthView;
}

void kl::gpu::clear(kl::dx::view::depth view, float depth, byte stencil) {
	m_Context->ClearDepthStencilView(view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, stencil);
}
