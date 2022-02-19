#include "KrimzLib/dx/fbuffer.h"


// Constructor
kl::fbuffer::fbuffer(IDXGISwapChain* chain, ID3D11Device* dev, ID3D11DeviceContext* devcon, int width, int height) {
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
    dev->CreateRenderTargetView(buffAddrs, nullptr, &view);
    if (!view) {
        std::cout << "DirectX: Could not create a backbuffer!";
        std::cin.get();
        exit(69);
    }

    // Cleanup
    buffAddrs->Release();
}

// Destructor
kl::fbuffer::~fbuffer() {
    view->Release();
}

// Returns the view pointer
ID3D11RenderTargetView* kl::fbuffer::getView() {
    return view;
}

// Clears the buffer
void kl::fbuffer::clear(const kl::vec4& color) {
    devcon->ClearRenderTargetView(view, (float*)&color);
}
