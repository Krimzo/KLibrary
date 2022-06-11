#include "gpu/gpu.h"

#include "utility/console.h"


ID3D11RasterizerState* kl::gpu::newRasterState(D3D11_RASTERIZER_DESC* desc) {
	ID3D11RasterizerState* rasterState = nullptr;
	m_Device->CreateRasterizerState(desc, &rasterState);
	kl::console::error(!rasterState, "Failed to create rasterizer state");

	m_Children.insert(rasterState);
	return rasterState;
}

ID3D11RasterizerState* kl::gpu::newRasterState(bool wireframe, bool cull, bool cullBack) {
	D3D11_RASTERIZER_DESC rasterStateDesc = {};
	rasterStateDesc.FillMode = wireframe ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID;
	rasterStateDesc.CullMode = cull ? (cullBack ? D3D11_CULL_BACK : D3D11_CULL_FRONT) : D3D11_CULL_NONE;
	rasterStateDesc.FrontCounterClockwise = true;
	rasterStateDesc.MultisampleEnable = true;
	rasterStateDesc.AntialiasedLineEnable = true;

	return newRasterState(&rasterStateDesc);
}

void kl::gpu::bind(ID3D11RasterizerState* state) {
	m_Context->RSSetState(state);
}
