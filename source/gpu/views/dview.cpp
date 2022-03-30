#include "gpu/gpu.h"


ID3D11DepthStencilView* kl::gpu::newDepthView(ID3D11Texture2D* tex, D3D11_DEPTH_STENCIL_VIEW_DESC* desc) {
    // Creating the render target view
    ID3D11DepthStencilView* depthView = nullptr;
    device->CreateDepthStencilView(tex, desc, &depthView);
    if (!depthView) {
        std::cout << "DirectX: Could not create a backbuffer!";
        std::cin.get();
        exit(69);
    }

    // Saving child
    children.push_back(depthView);

    // Return
    return depthView;
}

void kl::gpu::clear(ID3D11DepthStencilView* view, float depth, byte stencil) {
    devcon->ClearDepthStencilView(view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, stencil);
}
