#include "gpu/gpu.h"


ID3D11DepthStencilState* kl::gpu::newDepthState(D3D11_DEPTH_STENCIL_DESC* desc) {
    // State creation
    ID3D11DepthStencilState* depthState = nullptr;
    device->CreateDepthStencilState(desc, &depthState);
    if (!depthState) {
        std::cout << "DirectX: Could not create a write depth/stencil state!";
        std::cin.get();
        exit(69);
    }

    // Saving child
    children.push_back(depthState);

    // Return
    return depthState;
}

ID3D11DepthStencilState* kl::gpu::newDepthState(bool depth, bool stencil, bool mask) {
    // Depth descriptor
    D3D11_DEPTH_STENCIL_DESC depthDesc = {};

    // Depth testing
    if (depth) {
        depthDesc.DepthEnable = true;
        depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        depthDesc.DepthFunc = D3D11_COMPARISON_LESS;
    }

    // Stencil testing
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

    // Return
    return this->newDepthState(&depthDesc);
}

void kl::gpu::bind(ID3D11DepthStencilState* state) {
    devcon->OMSetDepthStencilState(state, 0xFF);
}
