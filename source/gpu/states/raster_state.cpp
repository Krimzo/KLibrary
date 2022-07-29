#include "gpu/gpu.h"

#include "utility/console.h"


kl::dx::state::raster kl::gpu::newRasterState(kl::dx::state::desc::raster* desc)
{
	kl::dx::state::raster rasterState = nullptr;
	m_Device->CreateRasterizerState(desc, &rasterState);
	kl::console::error(!rasterState, "Failed to create rasterizer state");

	m_Children.insert(rasterState);
	return rasterState;
}

kl::dx::state::raster kl::gpu::newRasterState(bool wireframe, bool cull, bool cullBack)
{
	kl::dx::state::desc::raster rasterStateDesc = {};
	rasterStateDesc.FillMode = wireframe ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID;
	rasterStateDesc.CullMode = cull ? (cullBack ? D3D11_CULL_BACK : D3D11_CULL_FRONT) : D3D11_CULL_NONE;
	rasterStateDesc.FrontCounterClockwise = true;
	rasterStateDesc.MultisampleEnable = true;
	rasterStateDesc.AntialiasedLineEnable = true;

	return newRasterState(&rasterStateDesc);
}

void kl::gpu::bind(kl::dx::state::raster state)
{
	m_Context->RSSetState(state);
}
