#include "gpu/gpu.h"

#include "utility/console.h"


kl::dx::DepthView kl::GPU::newDepthView(dx::Texture texture, dx::DepthViewDesc* descriptor) {
	dx::DepthView view = nullptr;

	m_Device->CreateDepthStencilView(texture, descriptor, &view);
	Assert(!view, "Failed to create depth stencil view");

	m_Children.insert(view);

	return view;
}

void kl::GPU::clearDepthView(dx::DepthView view, float depth, byte stencil) {
	m_Context->ClearDepthStencilView(view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, stencil);
}
