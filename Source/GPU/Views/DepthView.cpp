#include "GPU/GPU.h"

#include "Utility/Console.h"


kl::dx::DepthView kl::GPU::newDepthView(dx::Texture texture, dx::DepthViewDesc* descriptor) {
	dx::DepthView view = nullptr;

	long result = m_Device->CreateDepthStencilView(texture, descriptor, &view);
	if (Warning(!view, Format("Failed to create depth stencil view. Result: 0x", std::hex, result))) {
		return nullptr;
	}

	m_Children.insert(view);

	return view;
}

void kl::GPU::clearDepthView(dx::DepthView view, float depth, byte stencil) {
	m_Context->ClearDepthStencilView(view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, stencil);
}
