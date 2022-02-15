#include "KrimzLib/dx/fbuffer.h"


// Constructor
kl::fbuffer::fbuffer(IDXGISwapChain* chain, ID3D11Device* dev, ID3D11DeviceContext* devcon, int width, int height, int msaa) {
    // Saving devcon
    this->devcon = devcon;

    // Getting the back buffer address
    ID3D11Texture2D* buffAddrs = nullptr;
    chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffAddrs);
    if (!buffAddrs) {
        std::cout << "DirectX: Could not get back buffer address!";
        std::cin.get();
        exit(69);
    }

    // Creating the backbuffer
    dev->CreateRenderTargetView(buffAddrs, nullptr, &backBuff);
    if (!backBuff) {
        std::cout << "DirectX: Could not create a backbuffer!";
        std::cin.get();
        exit(69);
    }

    // Creating depth/stencil states
    D3D11_DEPTH_STENCIL_DESC depthDesc_e = {};
    depthDesc_e.DepthEnable = true;
    depthDesc_e.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthDesc_e.DepthFunc = D3D11_COMPARISON_LESS;
    depthDesc_e.StencilEnable = false;
    depthDesc_e.StencilEnable = true;
    depthDesc_e.StencilReadMask = 0xFF;
    depthDesc_e.StencilWriteMask = 0xFF;
    depthDesc_e.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthDesc_e.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    depthDesc_e.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthDesc_e.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    depthDesc_e.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthDesc_e.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    depthDesc_e.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthDesc_e.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    dev->CreateDepthStencilState(&depthDesc_e, &dsState_enabled);
    if (!dsState_enabled) {
        std::cout << "DirectX: Could not create a depth/stencil state!";
        std::cin.get();
        exit(69);
    }
    D3D11_DEPTH_STENCIL_DESC depthDesc_d = {};
    depthDesc_d.DepthEnable = false;
    depthDesc_d.StencilEnable = false;
    dev->CreateDepthStencilState(&depthDesc_d, &dsState_disabled);
    if (!dsState_disabled) {
        std::cout << "DirectX: Could not create a depth/stencil state!";
        std::cin.get();
        exit(69);
    }

    // Creating depth/stencil textures
    D3D11_TEXTURE2D_DESC depthTexDesc = {};
    depthTexDesc.Width = width;
    depthTexDesc.Height = height;
    depthTexDesc.MipLevels = 1;
    depthTexDesc.ArraySize = 1;
    depthTexDesc.Format = DXGI_FORMAT_D32_FLOAT;
    depthTexDesc.SampleDesc.Count = msaa;
    depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
    depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    ID3D11Texture2D* depthTex = nullptr;
    dev->CreateTexture2D(&depthTexDesc, NULL, &depthTex);
    if (!depthTex) {
        std::cout << "DirectX: Could not create a depth/stencil buffer texture!";
        std::cin.get();
        exit(69);
    }

    // Creating depth/stencil buffers
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
    dsvDesc.Format = depthTexDesc.Format;
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
    dev->CreateDepthStencilView(depthTex, &dsvDesc, &dsBuff);
    if (!dsBuff) {
        std::cout << "DirectX: Could not create a depth/stencil buffer view!";
        std::cin.get();
        exit(69);
    }

    // Cleanup
    depthTex->Release();
    buffAddrs->Release();
}

// Destructor
kl::fbuffer::~fbuffer() {
    dsState_enabled->Release();
    dsState_disabled->Release();
    dsBuff->Release();
    backBuff->Release();
}

// Binds the buffer
void kl::fbuffer::bind(bool depthTest) {
    devcon->OMSetDepthStencilState(depthTest ? dsState_enabled : dsState_disabled, 1);
    devcon->OMSetRenderTargets(1, &backBuff, dsBuff);
}

// Clears the buffer
void kl::fbuffer::clearColor(const kl::vec4& color) {
    devcon->ClearRenderTargetView(backBuff, (float*)&color);
}
void kl::fbuffer::clearDepth() {
    devcon->ClearDepthStencilView(dsBuff, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
}
