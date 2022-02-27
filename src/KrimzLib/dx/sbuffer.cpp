#include "KrimzLib/dx/sbuffer.h"


// Constructor
kl::sbuffer::sbuffer(ID3D11Device* dev, ID3D11DeviceContext* devcon, uint32_t size) {
    // Saving the devcon
    this->devcon = devcon;

    // Saving the size
    this->size = size;

    // Creating the depth state
    D3D11_DEPTH_STENCIL_DESC dsDesc_e = {};
    dsDesc_e.DepthEnable = true;
    dsDesc_e.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsDesc_e.DepthFunc = D3D11_COMPARISON_LESS;
    dsDesc_e.StencilEnable = false;
    dev->CreateDepthStencilState(&dsDesc_e, &state);
    if (!state) {
        std::cout << "DirectX: Could not create a shadow depth state!";
        std::cin.get();
        exit(69);
    }

    // Creating buffer texture
    D3D11_TEXTURE2D_DESC texDesc = {};
    texDesc.Width = size;
    texDesc.Height = size;
    texDesc.MipLevels = 1;
    texDesc.ArraySize = 1;
    texDesc.Format = DXGI_FORMAT_R32_TYPELESS;
    texDesc.SampleDesc.Count = 1;
    texDesc.Usage = D3D11_USAGE_DEFAULT;
    texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
    ID3D11Texture2D* tex = nullptr;
    dev->CreateTexture2D(&texDesc, nullptr, &tex);
    if (!tex) {
        std::cout << "DirectX: Could not create a shadow depth buffer texture!";
        std::cin.get();
        exit(69);
    }

    // Creating the depth view
    D3D11_DEPTH_STENCIL_VIEW_DESC depthViewDesc = {};
    depthViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
    depthViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    dev->CreateDepthStencilView(tex, &depthViewDesc, &dview);
    if (!dview) {
        std::cout << "DirectX: Could not create a shadow depth view!";
        std::cin.get();
        exit(69);
    }

    // Creating the shader resource view
    D3D11_SHADER_RESOURCE_VIEW_DESC resViewDesc = {};
    resViewDesc.Format = DXGI_FORMAT_R32_FLOAT;
    resViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    resViewDesc.Texture2D.MipLevels = 1;
    dev->CreateShaderResourceView(tex, &resViewDesc, &sview);
    if (!sview) {
        std::cout << "DirectX: Could not create a shadow shader view!";
        std::cin.get();
        exit(69);
    }

    // Sampler creation
    D3D11_SAMPLER_DESC sampDesc = {};
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
    sampDesc.BorderColor[0] = 1.0f;
    sampDesc.BorderColor[1] = 1.0f;
    sampDesc.BorderColor[2] = 1.0f;
    sampDesc.BorderColor[3] = 1.0f;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_LESS;
    dev->CreateSamplerState(&sampDesc, &samp);
    if (!samp) {
        std::cout << "DirectX: Could not create a shadow sampler!";
        std::cin.get();
        exit(69);
    }

    // Cleanup
    tex->Release();
}

// Destructor
kl::sbuffer::~sbuffer() {
    samp->Release();
    state->Release();
    dview->Release();
    sview->Release();
}

// Binds the shadow states
void kl::sbuffer::bindTarget() {
    devcon->OMSetRenderTargets(0, nullptr, dview);
    devcon->OMSetDepthStencilState(state, 0);
}

// Binds shadows sampler and texture
void kl::sbuffer::bindTexture(int sampSlot, int texSlot) {
    devcon->PSSetSamplers(sampSlot, 1, &samp);
    devcon->PSSetShaderResources(texSlot, 1, &sview);
}

// Sets the proper viewport
void kl::sbuffer::fixViewport() {
    D3D11_VIEWPORT viewport = {};
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    viewport.Width = float(size);
    viewport.Height = float(size);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    devcon->RSSetViewports(1, &viewport);
}

// Returns the size
uint32_t kl::sbuffer::getSize() {
    return this->size;
}

// Clears the buffer
void kl::sbuffer::clear() {
    devcon->ClearDepthStencilView(dview, D3D11_CLEAR_DEPTH, 1.0f, 0);
}
