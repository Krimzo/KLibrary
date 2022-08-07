#include "gpu/gpu.h"

#include "utility/console.h"


kl::dx::TargetView kl::GPU::newTargetView(dx::Texture texture, dx::TargetViewDesc* descriptor) {
	dx::TargetView view = nullptr;

	m_Device->CreateRenderTargetView(texture, descriptor, &view);
	Assert(!view, "Failed to create render target view");

	m_Children.insert(view);

	return view;
}

void kl::GPU::clearTargetView(dx::TargetView view, const Float4& color) {
	m_Context->ClearRenderTargetView(view, color.data);
}
