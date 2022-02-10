#include "KrimzLib/dx/shaders.h"


// Constructors
kl::shaders::shaders(ID3D11DeviceContext* devcon, ID3D10Blob* vsBlob, ID3D10Blob* psBlob, ID3D11VertexShader* vs, ID3D11PixelShader* ps) {
	this->devcon = devcon;
	this->vsBlob = vsBlob;
	this->psBlob = psBlob;
	this->vs = vs;
	this->ps = ps;
}

// Destructor
kl::shaders::~shaders() {
	if (vsBlob) vsBlob->Release();
	if (psBlob) psBlob->Release();
	if (vs) vs->Release();
	if (ps) ps->Release();
}

// Binds the shaders
void kl::shaders::bind() const {
	devcon->VSSetShader(this->vs, nullptr, 0);
	devcon->PSSetShader(this->ps, nullptr, 0);
}
