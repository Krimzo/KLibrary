#include "KrimzLib/dx/cbuffer.h"


// Constructor
kl::cbuffer::cbuffer(ID3D11Device* dev, ID3D11DeviceContext* devcon, uint32_t byteSize) {
    // Saving the devcon
    this->devcon = devcon;

    // Saving size
    this->size = byteSize;

    // Checking the size
    if (byteSize % 16 != 0) {
        std::cout << "DirectX: Constant buffer size has to be a multiple of 16!";
        std::cin.get();
        exit(69);
    }

    // Buffer descriptor creation
    D3D11_BUFFER_DESC buffDesc = {};
    buffDesc.ByteWidth = byteSize;
    buffDesc.Usage = D3D11_USAGE_DYNAMIC;
    buffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    buffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    buffDesc.MiscFlags = NULL;
    buffDesc.StructureByteStride = 0;

    // Buffer creation
    dev->CreateBuffer(&buffDesc, nullptr, &buff);
    if (!buff) {
        std::cout << "DirectX: Could not create a constant buffer!";
        std::cin.get();
        exit(69);
    }
}

// Destructor
kl::cbuffer::~cbuffer() {
    buff->Release();
}

// Binds the buffer
void kl::cbuffer::bind(bool pixlSha, int slot) {
    if (pixlSha) {
        devcon->PSSetConstantBuffers(slot, 1, &buff);
    }
    else {
        devcon->VSSetConstantBuffers(slot, 1, &buff);
    }
}

// Sets the data of the buffer
void kl::cbuffer::setData(void* data) {
    D3D11_MAPPED_SUBRESOURCE ms = {};
    devcon->Map(this->buff, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
    memcpy(ms.pData, data, this->size);
    devcon->Unmap(this->buff, NULL);
}
