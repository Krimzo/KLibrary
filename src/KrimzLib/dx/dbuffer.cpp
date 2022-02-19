#include "KrimzLib/dx/dbuffer.h"


// Constructor
kl::dbuffer::dbuffer(ID3D11Device* dev, ID3D11DeviceContext* devcon, int width, int height) {
    // Saving the devcon
    this->devcon = devcon;

    // Creating the enabled state
    D3D11_DEPTH_STENCIL_DESC dsDesc_e = {};
    dsDesc_e.DepthEnable = true;
    dsDesc_e.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsDesc_e.DepthFunc = D3D11_COMPARISON_LESS;
    dsDesc_e.StencilEnable = false;
    dsDesc_e.StencilEnable = true;
    dsDesc_e.StencilReadMask = 0xFF;
    dsDesc_e.StencilWriteMask = 0xFF;
    dsDesc_e.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dsDesc_e.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    dsDesc_e.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    dsDesc_e.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    dsDesc_e.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dsDesc_e.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    dsDesc_e.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    dsDesc_e.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    dev->CreateDepthStencilState(&dsDesc_e, &stateEnabled);
    if (!stateEnabled) {
        std::cout << "DirectX: Could not create a depth/stencil state!";
        std::cin.get();
        exit(69);
    }

    // Creating the disabled state
    D3D11_DEPTH_STENCIL_DESC dsDesc_d = {};
    dsDesc_d.DepthEnable = false;
    dsDesc_d.StencilEnable = false;
    dev->CreateDepthStencilState(&dsDesc_d, &stateDisabled);
    if (!stateDisabled) {
        std::cout << "DirectX: Could not create a depth/stencil state!";
        std::cin.get();
        exit(69);
    }

    // Creating buffer texture
    D3D11_TEXTURE2D_DESC dsTexDesc = {};
    dsTexDesc.Width = width;
    dsTexDesc.Height = height;
    dsTexDesc.MipLevels = 1;
    dsTexDesc.ArraySize = 1;
    dsTexDesc.Format = DXGI_FORMAT_D32_FLOAT;
    dsTexDesc.SampleDesc.Count = 1;
    dsTexDesc.Usage = D3D11_USAGE_DEFAULT;
    dsTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    ID3D11Texture2D* dsTex = nullptr;
    dev->CreateTexture2D(&dsTexDesc, NULL, &dsTex);
    if (!dsTex) {
        std::cout << "DirectX: Could not create a depth/stencil buffer texture!";
        std::cin.get();
        exit(69);
    }

    // Creating the texture view
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
    dsvDesc.Format = dsTexDesc.Format;
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
    dev->CreateDepthStencilView(dsTex, &dsvDesc, &view);
    if (!view) {
        std::cout << "DirectX: Could not create a depth/stencil buffer view!";
        std::cin.get();
        exit(69);
    }

    // Cleanup
    dsTex->Release();
}

// Destructor
kl::dbuffer::~dbuffer() {
    stateEnabled->Release();
    stateDisabled->Release();
    view->Release();
}

// Binds the depth/stencil buffer
void kl::dbuffer::setState(bool enabled) {
    devcon->OMSetDepthStencilState(enabled ? stateEnabled : stateDisabled, 1);
}

// Returns the view pointer
ID3D11DepthStencilView* kl::dbuffer::getView() {
    return view;
}

// Clears the buffer
void kl::dbuffer::clear() {
    devcon->ClearDepthStencilView(view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
}
