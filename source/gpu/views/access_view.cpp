#include "gpu/gpu.h"

#include "utility/console.h"


ID3D11UnorderedAccessView* kl::gpu::newAccessView(ID3D11Texture2D* tex, D3D11_UNORDERED_ACCESS_VIEW_DESC* desc) {
	ID3D11UnorderedAccessView* accessView = nullptr;
	m_Device->CreateUnorderedAccessView(tex, desc, &accessView);
	kl::console::error(!accessView, "Failed to create unordered access view");

	m_Children.insert(accessView);
	return accessView;
}

void kl::gpu::bindComputeAccessView(ID3D11UnorderedAccessView* view, uint slot, uint* initalCounts) {
	m_Context->CSSetUnorderedAccessViews(slot, 1, &view, initalCounts);
}
