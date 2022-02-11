#include "KrimzLib/dx/texture.h"

#include <vector>


// Constructor
kl::texture::texture(ID3D11Device* dev, ID3D11DeviceContext* devcon, const kl::image& img) {
	// Saving the devcon
	this->devcon = devcon;

    // Image copy
    kl::image imgCpy = img;
    imgCpy.flipVertical();

    // Allocating the temp pixel memory
    std::vector<uint32_t> pixelData(imgCpy.getWidth() * imgCpy.getHeight());

    // Reading and storing the pixel data
    const kl::color* imagePointer = (kl::color*)imgCpy.pointer();
    for (int i = 0; i < imgCpy.getWidth() * imgCpy.getHeight(); i++) {
        pixelData[i] = 0xFF;
        pixelData[i] = (pixelData[i] << 8) + int(imagePointer[i].b);
        pixelData[i] = (pixelData[i] << 8) + int(imagePointer[i].g);
        pixelData[i] = (pixelData[i] << 8) + int(imagePointer[i].r);
    }

    // Texture descriptor creation
    D3D11_TEXTURE2D_DESC texDesc = {};
    texDesc.Width = imgCpy.getWidth();
    texDesc.Height = imgCpy.getHeight();
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
    texData.SysMemPitch = imgCpy.getWidth() * sizeof(unsigned int);

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

// Destructor
kl::texture::~texture() {
	view->Release();
}

// Binds the texture
void kl::texture::bind(int slot) {
	devcon->PSSetShaderResources(slot, 1, &view);
}
