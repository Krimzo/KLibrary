#include "KrimzLib/dx/dbuffer.h"


// Constructor
kl::dbuffer::dbuffer(ID3D11Device* dev, ID3D11DeviceContext* devcon, int width, int height) {
    // Saving the devcon
    this->devcon = devcon;

    // Creating the default state
    D3D11_DEPTH_STENCIL_DESC dsDesc_def = {};
    dsDesc_def.DepthEnable = true;
    dsDesc_def.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsDesc_def.DepthFunc = D3D11_COMPARISON_LESS;
    dsDesc_def.StencilEnable = false;
    dev->CreateDepthStencilState(&dsDesc_def, &stateDefault);
    if (!stateDefault) {
        std::cout << "DirectX: Could not create a default depth/stencil state!";
        std::cin.get();
        exit(69);
    }

    // Creating the disabled state
    D3D11_DEPTH_STENCIL_DESC dsDesc_dis = {};
    dsDesc_dis.DepthEnable = false;
    dsDesc_dis.StencilEnable = false;
    dev->CreateDepthStencilState(&dsDesc_dis, &stateDisabled);
    if (!stateDisabled) {
        std::cout << "DirectX: Could not create a disabled depth/stencil state!";
        std::cin.get();
        exit(69);
    }

    // Creating the disabled state
    D3D11_DEPTH_STENCIL_DESC dsDesc_wri = {};
    dsDesc_wri.DepthEnable = true;
    dsDesc_wri.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsDesc_wri.DepthFunc = D3D11_COMPARISON_LESS;
    dsDesc_wri.StencilEnable = true;
    dsDesc_wri.StencilReadMask = 0xFF;
    dsDesc_wri.StencilWriteMask = 0xFF;
    dsDesc_wri.FrontFace.StencilFailOp = D3D11_STENCIL_OP_REPLACE;
    dsDesc_wri.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_REPLACE;
    dsDesc_wri.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
    dsDesc_wri.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    dsDesc_wri.BackFace.StencilFailOp = D3D11_STENCIL_OP_REPLACE;
    dsDesc_wri.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_REPLACE;
    dsDesc_wri.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
    dsDesc_wri.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    dev->CreateDepthStencilState(&dsDesc_wri, &stateWrite);
    if (!stateWrite) {
        std::cout << "DirectX: Could not create a write depth/stencil state!";
        std::cin.get();
        exit(69);
    }

    // Creating the disabled state
    D3D11_DEPTH_STENCIL_DESC dsDesc_mas = {};
    dsDesc_mas.DepthEnable = false;
    dsDesc_mas.StencilEnable = true;
    dsDesc_mas.StencilReadMask = 0xFF;
    dsDesc_mas.StencilWriteMask = 0xFF;
    dsDesc_mas.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dsDesc_mas.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    dsDesc_mas.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    dsDesc_mas.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
    dsDesc_mas.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dsDesc_mas.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    dsDesc_mas.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    dsDesc_mas.BackFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
    dev->CreateDepthStencilState(&dsDesc_mas, &stateMask);
    if (!stateMask) {
        std::cout << "DirectX: Could not create a mask depth/stencil state!";
        std::cin.get();
        exit(69);
    }

    // Creating buffer texture
    D3D11_TEXTURE2D_DESC dsTexDesc = {};
    dsTexDesc.Width = width;
    dsTexDesc.Height = height;
    dsTexDesc.MipLevels = 1;
    dsTexDesc.ArraySize = 1;
    dsTexDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
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
    dev->CreateDepthStencilView(dsTex, nullptr, &view);
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
    stateDefault->Release();
    stateDisabled->Release();
    stateWrite->Release();
    stateMask->Release();
    view->Release();
}

// Binds the depth/stencil buffer
void kl::dbuffer::setState(kl::dbuffer::STATE state) {
    ID3D11DepthStencilState* toSet = nullptr;
    switch (state) {
    case kl::dbuffer::Disabled:
        toSet = stateDisabled;
        break;

    case kl::dbuffer::Write:
        toSet = stateWrite;
        break;

    case kl::dbuffer::Mask:
        toSet = stateMask;
        break;

    default:
        toSet = stateDefault;
        break;
    }
    devcon->OMSetDepthStencilState(toSet, 0xFF);
}

// Returns the view pointer
ID3D11DepthStencilView* kl::dbuffer::getView() {
    return view;
}

// Clears the buffer
void kl::dbuffer::clear() {
    devcon->ClearDepthStencilView(view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}
