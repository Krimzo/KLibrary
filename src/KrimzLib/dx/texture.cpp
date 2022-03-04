#include "KrimzLib/dx/texture.h"

#include <vector>


// Constructors
kl::texture::texture(ID3D11Device* dev, ID3D11DeviceContext* devcon, const kl::image& img) {
	// Saving the devcon
	this->devcon = devcon;

    // Getting the flipped image
    const kl::image flipped = img.flipV();

    // Texture descriptor creation
    D3D11_TEXTURE2D_DESC texDesc = {};
    texDesc.Width = img.width();
    texDesc.Height = img.height();
    texDesc.MipLevels = 1;
    texDesc.ArraySize = 1;
    texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    texDesc.SampleDesc.Count = 1;
    texDesc.Usage = D3D11_USAGE_DEFAULT;
    texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    // Texture data descriptor creation
    D3D11_SUBRESOURCE_DATA texData = {};
    texData.pSysMem = flipped.pointer();
    texData.SysMemPitch = img.width() * sizeof(uint32_t);

    // Texture creation
    ID3D11Texture2D* tex = nullptr;
    dev->CreateTexture2D(&texDesc, &texData, &tex);
    if (!tex) {
        std::cout << "DirectX: Could not create a 2D texture!";
        std::cin.get();
        exit(69);
    }

    // Texture view creation
    dev->CreateShaderResourceView(tex, nullptr, &view);
    if (!view) {
        std::cout << "DirectX: Could not create a 2D texture view!";
        std::cin.get();
        exit(69);
    }

    // Cleanup
    tex->Release();
}
kl::texture::texture(ID3D11Device* dev, ID3D11DeviceContext* devcon, const kl::image& front, const kl::image& back, const kl::image& left, const kl::image& right, const kl::image& top, const kl::image& bottom) {
    // Saving the devcon
    this->devcon = devcon;

    // Image size check
    if (!(front.size() == back.size() && front.size() == left.size() && front.size() == right.size() && front.size() == top.size() && front.size() == bottom.size())) {
        std::cout << "Texture: Sizes of the 6 given images do not match!";
        std::cin.get();
        exit(69);
    }

    // Texture descriptor creation
    D3D11_TEXTURE2D_DESC texDesc = {};
    texDesc.Width = front.width();
    texDesc.Height = front.height();
    texDesc.MipLevels = 1;
    texDesc.ArraySize = 6;
    texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    texDesc.SampleDesc.Count = 1;
    texDesc.Usage = D3D11_USAGE_DEFAULT;
    texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    texDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

    // Texture data descriptor creation
    D3D11_SUBRESOURCE_DATA texData[6] = {
        {  right.pointer(), front.width() * sizeof(uint32_t), 0 },
        {   left.pointer(), front.width() * sizeof(uint32_t), 0 },
        {    top.pointer(), front.width() * sizeof(uint32_t), 0 },
        { bottom.pointer(), front.width() * sizeof(uint32_t), 0 },
        {  front.pointer(), front.width() * sizeof(uint32_t), 0 },
        {   back.pointer(), front.width() * sizeof(uint32_t), 0 }
    };

    // Texture creation
    ID3D11Texture2D* tex = nullptr;
    dev->CreateTexture2D(&texDesc, texData, &tex);
    if (!tex) {
        std::cout << "DirectX: Could not create a 2D texture!";
        std::cin.get();
        exit(69);
    }

    // Texture view creation
    dev->CreateShaderResourceView(tex, nullptr, &view);
    if (!view) {
        std::cout << "DirectX: Could not create a 2D texture view!";
        std::cin.get();
        exit(69);
    }

    // Cleanup
    tex->Release();
}

// Destructor
kl::texture::~texture() {
	view->Release();
}

// Binds the texture
void kl::texture::bind(int slot) {
	devcon->PSSetShaderResources(slot, 1, &view);
}
