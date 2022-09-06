#include "GPU/GPU.h"

#include "Utility/Console.h"


kl::dx::DepthState kl::GPU::newDepthState(dx::DepthStateDesc* descriptor) {
	dx::DepthState state = nullptr;

	long result = m_Device->CreateDepthStencilState(descriptor, &state);
	if (Warning(!state, Format("Failed to create depth stencil state. Result: 0x", std::hex, result))) {
		return nullptr;
	}

	m_Children.insert(state);

	return state;
}

kl::dx::DepthState kl::GPU::newDepthState(bool depth, bool stencil, bool mask) {
	dx::DepthStateDesc descriptor = {};

	if (depth) {
		descriptor.DepthEnable = true;
		descriptor.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		descriptor.DepthFunc = D3D11_COMPARISON_LESS;
	}

	if (stencil) {
		descriptor.StencilEnable = true;
		descriptor.StencilReadMask = 0xFF;
		descriptor.StencilWriteMask = 0xFF;

		if (mask) {
			descriptor.DepthEnable = false;
			descriptor.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			descriptor.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
			descriptor.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			descriptor.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
			descriptor.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			descriptor.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
			descriptor.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			descriptor.BackFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
		}
		else {
			descriptor.FrontFace.StencilFailOp = D3D11_STENCIL_OP_REPLACE;
			descriptor.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_REPLACE;
			descriptor.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
			descriptor.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
			descriptor.BackFace.StencilFailOp = D3D11_STENCIL_OP_REPLACE;
			descriptor.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_REPLACE;
			descriptor.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
			descriptor.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		}
	}

	return newDepthState(&descriptor);
}

void kl::GPU::bindDepthState(dx::DepthState state) {
	m_Context->OMSetDepthStencilState(state, 0xFF);
}
