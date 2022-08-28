#include "GPU/GPU.h"

#include "Utility/Console.h"


kl::dx::TargetView kl::GPU::newTargetView(dx::Texture texture, dx::TargetViewDesc* descriptor) {
	dx::TargetView view = nullptr;

	m_Device->CreateRenderTargetView(texture, descriptor, &view);
	if (Warning(!view, "Failed to create render target view")) {
		return nullptr;
	}

	m_Children.insert(view);

	return view;
}

void kl::GPU::clearTargetView(dx::TargetView view, const Float4& values) {
	m_Context->ClearRenderTargetView(view, values.data);
}
