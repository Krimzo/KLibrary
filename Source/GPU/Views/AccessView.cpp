#include "GPU/GPU.h"

#include "Utility/Console.h"


kl::dx::AccessView kl::GPU::newAccessView(dx::Resource resource, dx::AccessViewDesc* descriptor) {
	dx::AccessView view = nullptr;

	long result = m_Device->CreateUnorderedAccessView(resource, descriptor, &view);
	if (Warning(!view, Format("Failed to create unordered access view. Result: 0x", std::hex, result))) {
		return nullptr;
	}

	m_Children.insert(view);

	return view;
}

void kl::GPU::bindComputeAccessView(dx::AccessView view, uint slot, uint* initalCounts) {
	m_Context->CSSetUnorderedAccessViews(slot, 1, &view, initalCounts);
}
