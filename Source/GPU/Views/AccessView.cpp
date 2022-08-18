#include "GPU/GPU.h"

#include "Utility/Console.h"


kl::dx::AccessView kl::GPU::newAccessView(dx::Texture texture, dx::AccessViewDesc* descriptor) {
	dx::AccessView view = nullptr;

	m_Device->CreateUnorderedAccessView(texture, descriptor, &view);
	if (Warning(!view, "Failed to create unordered access view")) {
		return nullptr;
	}

	m_Children.insert(view);

	return view;
}

void kl::GPU::bindComputeAccessView(dx::AccessView view, uint slot, uint* initalCounts) {
	m_Context->CSSetUnorderedAccessViews(slot, 1, &view, initalCounts);
}
