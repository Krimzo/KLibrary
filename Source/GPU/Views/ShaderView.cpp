#include "GPU/GPU.h"

#include "Utility/Console.h"


kl::dx::ShaderView kl::GPU::newShaderView(dx::Resource resource, dx::ShaderViewDesc* descriptor) {
	dx::ShaderView view = nullptr;

	long result = m_Device->CreateShaderResourceView(resource, descriptor, &view);
	if (Warning(!view, Format("Failed to create shader view. Result: 0x", std::hex, result))) {
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
