#include "KrimzLib/dx/cbuffer.h"


// Constructor
kl::cbuffer::cbuffer(ID3D11DeviceContext* devcon, ID3D11Buffer* buff, int byteSize) {
    // Saving the devcon
    this->devcon = devcon;

    // Saving the buffer
    this->buff = buff;

    // Saving the buffer size
    this->size = byteSize;
}

// Destructor
kl::cbuffer::~cbuffer() {
    buff->Release();
}

// Binds the buffer
void kl::cbuffer::bind(const int slot) {
    ID3D11Buffer* tempCBuff = this->buff;
    devcon->VSSetConstantBuffers(slot, 1, &tempCBuff);
}

// Sets the data of the buffer
void kl::cbuffer::setData(void* data) {
    D3D11_MAPPED_SUBRESOURCE ms = {};
    devcon->Map(this->buff, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
    memcpy(ms.pData, data, this->size);
    devcon->Unmap(this->buff, NULL);
}
