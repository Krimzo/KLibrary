#include "KrimzLib/dx/ibuffer.h"

#include "KrimzLib/math/float4.h"


// Constructor
kl::ibuffer::ibuffer(ID3D11Device* dev, ID3D11DeviceContext* devcon, int width, int height) {
    // Saving the devcon
    this->devcon = devcon;

    // Saving the size
    this->width = width;
    this->height = height;

    // Creating the index texture
    D3D11_TEXTURE2D_DESC indTexDes = {};
    indTexDes.Width = width;
    indTexDes.Height = height;
    indTexDes.MipLevels = 1;
    indTexDes.ArraySize = 1;
    indTexDes.Format = DXGI_FORMAT_R32_FLOAT;
    indTexDes.SampleDesc.Count = 1;
    indTexDes.Usage = D3D11_USAGE_DEFAULT;
    indTexDes.BindFlags = D3D11_BIND_RENDER_TARGET;
    dev->CreateTexture2D(&indTexDes, nullptr, &indTex);
    if (!indTex) {
        std::cout << "DirectX: Could not create a picking texture!";
        std::cin.get();
        exit(69);
    }

    // Creating the index texture view
    dev->CreateRenderTargetView(indTex, nullptr, &view);
    if (!view) {
        std::cout << "DirectX: Could not create a picking render target!";
        std::cin.get();
        exit(69);
    }

    // Creating the staging texture
    D3D11_TEXTURE2D_DESC stagTexDes = {};
    stagTexDes.Width = 1;
    stagTexDes.Height = 1;
    stagTexDes.MipLevels = 1;
    stagTexDes.ArraySize = 1;
    stagTexDes.Format = indTexDes.Format;
    stagTexDes.SampleDesc.Count = 1;
    stagTexDes.Usage = D3D11_USAGE_STAGING;
    stagTexDes.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    dev->CreateTexture2D(&stagTexDes, nullptr, &stagTex);
    if (!stagTex) {
        std::cout << "DirectX: Could not create a staging texture!";
        std::cin.get();
        exit(69);
    }
}

// Destructor
kl::ibuffer::~ibuffer() {
    indTex->Release();
    stagTex->Release();
    view->Release();
}

// Returns the view pointer
ID3D11RenderTargetView* kl::ibuffer::getView() {
    return view;
}

// Clears the buffer
void kl::ibuffer::clear() {
    static const kl::float4 pickCol(-1, 0, 0, 0);
    devcon->ClearRenderTargetView(view, (float*)&pickCol);
}

// Returns the picking index
int kl::ibuffer::getIndex(const kl::int2& pos) {
    // Checking if the pos is in frame
    if (pos.x >= 0 && pos.x < this->width && pos.y >= 0 && pos.y < this->height) {
        // Copying the index texture pixel
        D3D11_BOX srcBox = {};
        srcBox.left = pos.x;
        srcBox.right = srcBox.left + 1;
        srcBox.top = pos.y;
        srcBox.bottom = srcBox.top + 1;
        srcBox.front = 0;
        srcBox.back = 1;
        devcon->CopySubresourceRegion(stagTex, 0, 0, 0, 0, indTex, 0, &srcBox);

        // Mapping the staging texture
        D3D11_MAPPED_SUBRESOURCE msr = {};
        devcon->Map(stagTex, 0, D3D11_MAP_READ, NULL, &msr);

        // Reading the bytes
        float index = 0;
        memcpy(&index, msr.pData, sizeof(float));

        // Unmapping the staging texture
        devcon->Unmap(stagTex, NULL);

        // Getting the index
        return int(index);
    }
    return -2;
}
