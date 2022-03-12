#include "KrimzLib/gpu/gpu.h"


ID3D11RasterizerState* kl::gpu::newRasterState(D3D11_RASTERIZER_DESC* desc) {
    // Raster creation
    ID3D11RasterizerState* rasterState = nullptr;
    device->CreateRasterizerState(desc, &rasterState);
    if (!rasterState) {
        std::cout << "DirectX: Could not create a raster state!";
        std::cin.get();
        exit(69);
    }

    // Saving child
    children.push_back(rasterState);

    // Return
    return rasterState;
}

ID3D11RasterizerState* kl::gpu::newRasterState(bool wireframe, bool cull, bool cullBack) {
    // Raster descriptor
    D3D11_RASTERIZER_DESC rasterStateDesc = {};
    rasterStateDesc.FillMode = wireframe ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID;
    rasterStateDesc.CullMode = cull ? (cullBack ? D3D11_CULL_BACK : D3D11_CULL_FRONT) : D3D11_CULL_NONE;
    rasterStateDesc.FrontCounterClockwise = true;
    rasterStateDesc.MultisampleEnable = true;
    rasterStateDesc.AntialiasedLineEnable = true;

    // Return
    return this->newRasterState(&rasterStateDesc);
}

// Binds the raster state
void kl::gpu::bind(ID3D11RasterizerState* state) {
    devcon->RSSetState(state);
}
