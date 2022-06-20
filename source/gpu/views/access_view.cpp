#include "gpu/gpu.h"

#include "utility/console.h"


kl::dx::view::access kl::gpu::newAccessView(kl::dx::texture tex, kl::dx::view::desc::access* desc) {
	kl::dx::view::access accessView = nullptr;
	m_Device->CreateUnorderedAccessView(tex, desc, &accessView);
	kl::console::error(!accessView, "Failed to create unordered access view");

	m_Children.insert(accessView);
	return accessView;
}

void kl::gpu::bindComputeAccessView(kl::dx::view::access view, uint slot, uint* initalCounts) {
	m_Context->CSSetUnorderedAccessViews(slot, 1, &view, initalCounts);
}
