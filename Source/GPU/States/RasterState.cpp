#include "GPU/GPU.h"

#include "Utility/Console.h"


kl::dx::RasterState kl::GPU::newRasterState(dx::RasterStateDesc* desc) {
	dx::RasterState rasterState = nullptr;

	m_Device->CreateRasterizerState(desc, &rasterState);
	if (Warning(!rasterState, "Failed to create rasterizer state")) {
		return nullptr;
	}

	m_Children.insert(rasterState);

	return rasterState;
}

kl::dx::RasterState kl::GPU::newRasterState(bool wireframe, bool cull, bool cullBack) {
	dx::RasterStateDesc rasterStateDesc = {};

	rasterStateDesc.FillMode = wireframe ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID;
	rasterStateDesc.CullMode = cull ? (cullBack ? D3D11_CULL_BACK : D3D11_CULL_FRONT) : D3D11_CULL_NONE;
	rasterStateDesc.FrontCounterClockwise = true;
	rasterStateDesc.MultisampleEnable = true;
	rasterStateDesc.AntialiasedLineEnable = true;

	return newRasterState(&rasterStateDesc);
}

void kl::GPU::bindRasterState(dx::RasterState state) {
	m_Context->RSSetState(state);
}
