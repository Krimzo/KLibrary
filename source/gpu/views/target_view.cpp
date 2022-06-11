#include "gpu/gpu.h"

#include "utility/console.h"


ID3D11RenderTargetView* kl::gpu::newTargetView(ID3D11Texture2D* tex, D3D11_RENDER_TARGET_VIEW_DESC* desc) {
	ID3D11RenderTargetView* targetView = nullptr;
	m_Device->CreateRenderTargetView(tex, desc, &targetView);
	kl::console::error(!targetView, "Failed to create render target view");

	m_Children.insert(targetView);
	return targetView;
}

void kl::gpu::clear(ID3D11RenderTargetView* view, const kl::float4& color) {
	m_Context->ClearRenderTargetView(view, (float*)&color);
}
