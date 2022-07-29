#include "gpu/gpu.h"

#include "utility/console.h"


kl::dx::view::shader kl::gpu::newShaderView(kl::dx::texture tex, kl::dx::view::desc::shader* desc)
{
	kl::dx::view::shader shaderView = nullptr;
	m_Device->CreateShaderResourceView(tex, desc, &shaderView);
	kl::console::error(!shaderView, "Failed to create shader view");

	m_Children.insert(shaderView);
	return shaderView;
}

void kl::gpu::bindPixelShaderView(kl::dx::view::shader view, uint slot)
{
	m_Context->PSSetShaderResources(slot, 1, &view);
}
void kl::gpu::bindComputeShaderView(kl::dx::view::shader view, uint slot)
{
	m_Context->CSSetShaderResources(slot, 1, &view);
}
