#include "gpu/gpu.h"

#include "utility/console.h"


kl::dx::view::target kl::gpu::newTargetView(kl::dx::texture tex, kl::dx::view::desc::target* desc) {
	kl::dx::view::target targetView = nullptr;
	m_Device->CreateRenderTargetView(tex, desc, &targetView);
	kl::console::error(!targetView, "Failed to create render target view");

	m_Children.insert(targetView);
	return targetView;
}

void kl::gpu::clear(kl::dx::view::target view, const kl::float4& color) {
	m_Context->ClearRenderTargetView(view, (float*)&color);
}
