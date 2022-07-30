#include "gpu/gpu.h"

#include "utility/console.h"


kl::dx::state::depth kl::gpu::newDepthState(kl::dx::state::desc::depth* desc) {
	kl::dx::state::depth depthState = nullptr;
	m_Device->CreateDepthStencilState(desc, &depthState);
	kl::console::error(!depthState, "Failed to create depth stencil state");

	m_Children.insert(depthState);
	return depthState;
}

kl::dx::state::depth kl::gpu::newDepthState(bool depth, bool stencil, bool mask) {
	kl::dx::state::desc::depth depthDesc = {};

	if (depth) {
		depthDesc.DepthEnable = true;
		depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthDesc.DepthFunc = D3D11_COMPARISON_LESS;
	}

	if (stencil) {
		depthDesc.StencilEnable = true;
		depthDesc.StencilReadMask = 0xFF;
		depthDesc.StencilWriteMask = 0xFF;
		if (mask) {
			depthDesc.DepthEnable = false;
			depthDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			depthDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
			depthDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			depthDesc.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
			depthDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			depthDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
			depthDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			depthDesc.BackFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
		}
		else {
			depthDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_REPLACE;
			depthDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_REPLACE;
			depthDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
			depthDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
			depthDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_REPLACE;
			depthDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_REPLACE;
			depthDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
			depthDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		}
	}

	return newDepthState(&depthDesc);
}

void kl::gpu::bind(kl::dx::state::depth state) {
	m_Context->OMSetDepthStencilState(state, 0xFF);
}
