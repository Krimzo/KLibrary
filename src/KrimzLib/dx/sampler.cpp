#include "KrimzLib/dx/sampler.h"


// Constructor
kl::sampler::sampler(ID3D11Device* dev, ID3D11DeviceContext* devcon, bool linear, bool mirror) {
	// Saving the devcon
	this->devcon = devcon;

    // Sampler creation
    D3D11_SAMPLER_DESC sampDesc = {};
    sampDesc.Filter = linear ? D3D11_FILTER_MIN_MAG_MIP_LINEAR : D3D11_FILTER_MIN_MAG_MIP_POINT;
    sampDesc.AddressU = mirror ? D3D11_TEXTURE_ADDRESS_MIRROR : D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = mirror ? D3D11_TEXTURE_ADDRESS_MIRROR : D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = mirror ? D3D11_TEXTURE_ADDRESS_MIRROR : D3D11_TEXTURE_ADDRESS_WRAP;
    dev->CreateSamplerState(&sampDesc, &state);
    if (!state) {
        std::cout << "DirectX: Could not create a sampler state!";
        std::cin.get();
        exit(69);
    }
}

// Destructor
kl::sampler::~sampler() {
	state->Release();
}

// Binds the sampler
void kl::sampler::bind(int slot) {
	devcon->PSSetSamplers(slot, 1, &state);
}
