#include "KrimzLib/dx/texture.h"


// Constructor
kl::texture::texture(ID3D11DeviceContext* devcon, ID3D11ShaderResourceView* view) {
	this->devcon = devcon;
	this->view = view;
}

// Destructor
kl::texture::~texture() {
	if (view) view->Release();
}

// Binds the texture
void kl::texture::bind(int slot) const {
	ID3D11ShaderResourceView* tempView = this->view;
	devcon->PSSetShaderResources(slot, 1, &tempView);
}
