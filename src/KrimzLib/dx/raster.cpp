#include "KrimzLib/dx/raster.h"


// Constructor
kl::raster::raster(ID3D11Device* dev, ID3D11DeviceContext* devcon, bool wireframe, bool cull, bool cullBack) {
    // Saving the devcon
    this->devcon = devcon;

    // Raster state creation
    D3D11_RASTERIZER_DESC rasterStateDesc = {};
    rasterStateDesc.FillMode = wireframe ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID;
    rasterStateDesc.CullMode = cull ? (cullBack ? D3D11_CULL_BACK : D3D11_CULL_FRONT) : D3D11_CULL_NONE;
    rasterStateDesc.FrontCounterClockwise = true;
    rasterStateDesc.MultisampleEnable = true;
    rasterStateDesc.AntialiasedLineEnable = true;
    dev->CreateRasterizerState(&rasterStateDesc, &state);
    if (!state) {
        std::cout << "DirectX: Could not create a raster state!";
        std::cin.get();
        exit(69);
    }
}

// Destructor
kl::raster::~raster() {
    state->Release();
}

// Binds the raster
void kl::raster::bind() const {
    devcon->RSSetState(state);
}
