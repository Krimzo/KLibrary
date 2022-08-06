#include "gpu/gpu.h"

#include "utility/console.h"


kl::dx::AccessView kl::GPU::newAccessView(dx::Texture texture, dx::AccessViewDesc* descriptor) {
	kl::dx::AccessView view = nullptr;

	m_Device->CreateUnorderedAccessView(texture, descriptor, &view);
	Assert(!view, "Failed to create unordered access view");

	m_Children.insert(view);

	return view;
}

void kl::GPU::bindComputeAccessView(dx::AccessView view, uint slot, uint* initalCounts) {
	m_Context->CSSetUnorderedAccessViews(slot, 1, &view, initalCounts);
}
