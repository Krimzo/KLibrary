#include "KrimzLib/dx/sampler.h"


// Constructor
kl::sampler::sampler(ID3D11DeviceContext* devcon, ID3D11SamplerState* state) {
	this->devcon = devcon;
	this->state = state;
}

// Destructor
kl::sampler::~sampler() {
	if (state) state->Release();
}

// Binds the sampler
void kl::sampler::bind(int slot) const {
	ID3D11SamplerState* tempState = this->state;
	devcon->PSSetSamplers(slot, 1, &tempState);
}
