#include "gpu/gpu.h"

#include "utility/console.h"


kl::dx::ShaderView kl::GPU::newShaderView(dx::Texture texture, dx::ShaderViewDesc* descriptor) {
	dx::ShaderView view = nullptr;

	m_Device->CreateShaderResourceView(texture, descriptor, &view);
	if (Warning(!view, "Failed to create shader view")) {
		return nullptr;
	}

	m_Children.insert(view);

	return view;
}

void kl::GPU::bindPixelShaderView(dx::ShaderView view, uint slot) {
	m_Context->PSSetShaderResources(slot, 1, &view);
}

void kl::GPU::bindComputeShaderView(dx::ShaderView view, uint slot) {
	m_Context->CSSetShaderResources(slot, 1, &view);
}
