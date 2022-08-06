#include "GPU/GPU.h"

#include "Utility/Console.h"


kl::dx::SamplerState kl::GPU::newSamplerState(dx::SamplerStateDesc* descriptor) {
	dx::SamplerState state = nullptr;

	m_Device->CreateSamplerState(descriptor, &state);
	Assert(!state, "Failed to create sampler state");

	m_Children.insert(state);

	return state;
}

kl::dx::SamplerState kl::GPU::newSamplerState(bool linear, bool mirror) {
	dx::SamplerStateDesc descriptor = {};

	descriptor.Filter = linear ? D3D11_FILTER_MIN_MAG_MIP_LINEAR : D3D11_FILTER_MIN_MAG_MIP_POINT;
	descriptor.AddressU = mirror ? D3D11_TEXTURE_ADDRESS_MIRROR : D3D11_TEXTURE_ADDRESS_WRAP;
	descriptor.AddressV = mirror ? D3D11_TEXTURE_ADDRESS_MIRROR : D3D11_TEXTURE_ADDRESS_WRAP;
	descriptor.AddressW = mirror ? D3D11_TEXTURE_ADDRESS_MIRROR : D3D11_TEXTURE_ADDRESS_WRAP;

	return newSamplerState(&descriptor);
}

void kl::GPU::bindSamplerState(dx::SamplerState state, uint slot) {
	m_Context->PSSetSamplers(slot, 1, &state);
}
