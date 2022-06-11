#include "gpu/gpu.h"

#include "utility/console.h"


ID3D11Texture2D* kl::gpu::newTextureBB() {
	ID3D11Texture2D* buffAddrs = nullptr;
	m_Chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffAddrs);
	kl::console::error(!buffAddrs, "Failed to create backbuffer texture");

	m_Children.insert(buffAddrs);
	return buffAddrs;
}

ID3D11Texture2D* kl::gpu::newTexture(D3D11_TEXTURE2D_DESC* desc, D3D11_SUBRESOURCE_DATA* subData) {
	ID3D11Texture2D* tex = nullptr;
	m_Device->CreateTexture2D(desc, subData, &tex);
	kl::console::error(!tex, "Failed to create texture");

	m_Children.insert(tex);
	return tex;
}

ID3D11Texture2D* kl::gpu::newTexture(const kl::image& img, bool enableUnorderedAccess) {
	const kl::image flipped = img.flipV();

	D3D11_TEXTURE2D_DESC texDesc = {};
	texDesc.Width = img.width();
	texDesc.Height = img.height();
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | (enableUnorderedAccess ? D3D11_BIND_UNORDERED_ACCESS : 0);

	D3D11_SUBRESOURCE_DATA texData = {};
	texData.pSysMem = flipped.data();
	texData.SysMemPitch = img.width() * sizeof(uint32_t);

	return newTexture(&texDesc, &texData);
}

ID3D11Texture2D* kl::gpu::newTexture(const kl::image& front, const kl::image& back, const kl::image& left, const kl::image& right, const kl::image& top, const kl::image& bottom) {
	kl::console::error(!(front.size() == back.size() && front.size() == left.size() && front.size() == right.size() && front.size() == top.size() && front.size() == bottom.size()),
		"Sizes of the 6 given images do not match");

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

	D3D11_SUBRESOURCE_DATA texData[6] = {
		{  right.data(), front.width() * sizeof(kl::color), 0 },
		{   left.data(), front.width() * sizeof(kl::color), 0 },
		{    top.data(), front.width() * sizeof(kl::color), 0 },
		{ bottom.data(), front.width() * sizeof(kl::color), 0 },
		{  front.data(), front.width() * sizeof(kl::color), 0 },
		{   back.data(), front.width() * sizeof(kl::color), 0 }
	};

	return newTexture(&texDesc, texData);
}

ID3D11Texture2D* kl::gpu::newTextureST(ID3D11Texture2D* tex, const kl::int2& size) {
	D3D11_TEXTURE2D_DESC oldDesc = {};
	tex->GetDesc(&oldDesc);

	D3D11_TEXTURE2D_DESC stagTexDes = {};
	stagTexDes.Width = (size.x > 0) ? size.x : oldDesc.Width;
	stagTexDes.Height = (size.y > 0) ? size.y : oldDesc.Height;
	stagTexDes.MipLevels = 1;
	stagTexDes.ArraySize = 1;
	stagTexDes.Format = oldDesc.Format;
	stagTexDes.SampleDesc.Count = 1;
	stagTexDes.Usage = D3D11_USAGE_STAGING;
	stagTexDes.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

	return newTexture(&stagTexDes);
}
