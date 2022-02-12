#include "KrimzLib/dx/texture.h"

#include <vector>


// Flips and fixed the image data
std::vector<uint32_t> ProcessImage(const kl::image& img, bool flip) {
    // Image copy
    kl::image imgCpy = img;
    
    // Vertical flip
    if (flip) {
        imgCpy.flipVertical();
    }

    // Allocating the temp pixel memory
    std::vector<uint32_t> pixelData(imgCpy.getWidth() * imgCpy.getHeight());

    // Reading and fixing the pixel data
    const kl::color* imagePointer = (kl::color*)imgCpy.pointer();
    for (int i = 0; i < imgCpy.getWidth() * imgCpy.getHeight(); i++) {
        pixelData[i] = 0xFF;
        pixelData[i] = (pixelData[i] << 8) + int(imagePointer[i].b);
        pixelData[i] = (pixelData[i] << 8) + int(imagePointer[i].g);
        pixelData[i] = (pixelData[i] << 8) + int(imagePointer[i].r);
    }

    // Returning
    return pixelData;
}

// Constructors
kl::texture::texture(ID3D11Device* dev, ID3D11DeviceContext* devcon, const kl::image& img) {
	// Saving the devcon
	this->devcon = devcon;

    // Fixing the image data
    auto pixelData = ProcessImage(img, true);

    // Texture descriptor creation
    D3D11_TEXTURE2D_DESC texDesc = {};
    texDesc.Width = img.getWidth();
    texDesc.Height = img.getHeight();
    texDesc.MipLevels = 1;
    texDesc.ArraySize = 1;
    texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    texDesc.SampleDesc.Count = 1;
    texDesc.Usage = D3D11_USAGE_DEFAULT;
    texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    texDesc.CPUAccessFlags = NULL;
    texDesc.MiscFlags = NULL;
    D3D11_SUBRESOURCE_DATA texData = {};
    texData.pSysMem = &pixelData[0];
    texData.SysMemPitch = img.getWidth() * sizeof(uint32_t);

    // Texture creation
    ID3D11Texture2D* tex = nullptr;
    dev->CreateTexture2D(&texDesc, &texData, &tex);
    if (!tex) {
        std::cout << "DirectX: Could not create a 2D texture!";
        std::cin.get();
        exit(69);
    }

    // Texture view creation
    D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc = {};
    viewDesc.Format = texDesc.Format;
    viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    viewDesc.Texture2D.MipLevels = 1;
    viewDesc.Texture2D.MostDetailedMip = 0;
    dev->CreateShaderResourceView(tex, &viewDesc, &view);
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
    if (!(front.getSize() == back.getSize() && front.getSize() == left.getSize() && front.getSize() == right.getSize() && front.getSize() == top.getSize() && front.getSize() == bottom.getSize())) {
        std::cout << "Texture: Sizes of the 6 given images do not match!";
        std::cin.get();
        exit(69);
    }

    // Image copy
    auto frontData = ProcessImage(front, false);
    auto backData = ProcessImage(back, false);
    auto leftData = ProcessImage(left, false);
    auto rightData = ProcessImage(right, false);
    auto topData = ProcessImage(top, false);
    auto bottomData = ProcessImage(bottom, false);

    // Texture descriptor creation
    D3D11_TEXTURE2D_DESC texDesc = {};
    texDesc.Width = front.getWidth();
    texDesc.Height = front.getHeight();
    texDesc.MipLevels = 1;
    texDesc.ArraySize = 6;
    texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    texDesc.SampleDesc.Count = 1;
    texDesc.Usage = D3D11_USAGE_DEFAULT;
    texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    texDesc.CPUAccessFlags = NULL;
    texDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
    D3D11_SUBRESOURCE_DATA texData[6] = {
        {  &rightData[0], front.getWidth() * sizeof(uint32_t), 0 },
        {   &leftData[0], front.getWidth() * sizeof(uint32_t), 0 },
        {    &topData[0], front.getWidth() * sizeof(uint32_t), 0 },
        { &bottomData[0], front.getWidth() * sizeof(uint32_t), 0 },
        {  &frontData[0], front.getWidth() * sizeof(uint32_t), 0 },
        {   &backData[0], front.getWidth() * sizeof(uint32_t), 0 }
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
    D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc = {};
    viewDesc.Format = texDesc.Format;
    viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
    viewDesc.Texture2D.MipLevels = 1;
    viewDesc.Texture2D.MostDetailedMip = 0;
    dev->CreateShaderResourceView(tex, &viewDesc, &view);
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
