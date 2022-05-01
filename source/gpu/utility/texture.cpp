#include "gpu/gpu.h"

#include "utility/console.h"


// Texture from backbuffer
ID3D11Texture2D* kl::gpu::newTextureBB() {
	// Getting the back buffer address
	ID3D11Texture2D* buffAddrs = nullptr;
	chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffAddrs);
	if (!buffAddrs) {
		kl::console::show();
		std::cout << "DirectX: Could not get back buffer address!";
		std::cin.get();
		exit(69);
	}

	// Saving child
	children.insert(buffAddrs);

	// Return
	return buffAddrs;
}

// Texture from descriptors
ID3D11Texture2D* kl::gpu::newTexture(D3D11_TEXTURE2D_DESC* desc, D3D11_SUBRESOURCE_DATA* subData) {
	// Texture creation
	ID3D11Texture2D* tex = nullptr;
	device->CreateTexture2D(desc, subData, &tex);
	if (!tex) {
		kl::console::show();
		std::cout << "DirectX: Could not create a 2D texture!";
		std::cin.get();
		exit(69);
	}

	// Saving child
	children.insert(tex);

	// Return
	return tex;
}

// Texture from image
ID3D11Texture2D* kl::gpu::newTexture(const kl::image& img) {
	// Getting the flipped image
	const kl::image flipped = img.flipV();

	// Texture descriptor creation
	D3D11_TEXTURE2D_DESC texDesc = {};
	texDesc.Width = img.width();
	texDesc.Height = img.height();
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	// Texture data descriptor creation
	D3D11_SUBRESOURCE_DATA texData = {};
	texData.pSysMem = flipped.pointer();
	texData.SysMemPitch = img.width() * sizeof(uint32_t);

	// Return
	return this->newTexture(&texDesc, &texData);
}

// Texture cube from images
ID3D11Texture2D* kl::gpu::newTexture(const kl::image& front, const kl::image& back, const kl::image& left, const kl::image& right, const kl::image& top, const kl::image& bottom) {
	// Image size check
	if (!(front.size() == back.size() && front.size() == left.size() && front.size() == right.size() && front.size() == top.size() && front.size() == bottom.size())) {
		kl::console::show();
		std::cout << "Texture: Sizes of the 6 given images do not match!";
		std::cin.get();
		exit(69);
	}

	// Texture descriptor creation
	D3D11_TEXTURE2D_DESC texDesc = {};
	texDesc.Width = front.width();
	texDesc.Height = front.height();
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 6;
	texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	// Texture data descriptor creation
	D3D11_SUBRESOURCE_DATA texData[6] = {
		{  right.pointer(), front.width() * sizeof(kl::color), 0 },
		{   left.pointer(), front.width() * sizeof(kl::color), 0 },
		{    top.pointer(), front.width() * sizeof(kl::color), 0 },
		{ bottom.pointer(), front.width() * sizeof(kl::color), 0 },
		{  front.pointer(), front.width() * sizeof(kl::color), 0 },
		{   back.pointer(), front.width() * sizeof(kl::color), 0 }
	};

	// Return
	return this->newTexture(&texDesc, texData);
}

// Staging texture
ID3D11Texture2D* kl::gpu::newTextureST(ID3D11Texture2D* tex, const kl::int2& size) {
	// Getting the given tex desc
	D3D11_TEXTURE2D_DESC oldDesc = {};
	tex->GetDesc(&oldDesc);

	// Staging texture descriptor
	D3D11_TEXTURE2D_DESC stagTexDes = {};
	stagTexDes.Width = (size.x > 0) ? size.x : oldDesc.Width;
	stagTexDes.Height = (size.y > 0) ? size.y : oldDesc.Height;
	stagTexDes.MipLevels = 1;
	stagTexDes.ArraySize = 1;
	stagTexDes.Format = oldDesc.Format;
	stagTexDes.SampleDesc.Count = 1;
	stagTexDes.Usage = D3D11_USAGE_STAGING;
	stagTexDes.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

	// Return
	return this->newTexture(&stagTexDes);
}
