#include "GPU/GPU.h"

#include "Utility/Console.h"


kl::dx::Texture kl::GPU::getBackBuffer() {
	dx::Texture buffer = nullptr;

	m_Chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer);
	if (Warning(!buffer, "Failed to get backbuffer texture")) {
		return nullptr;
	}

	m_Children.insert(buffer);

	return buffer;
}

kl::dx::Texture kl::GPU::newTexture(dx::TextureDesc* descriptor, dx::SubresDesc* subresourceData) {
	dx::Texture texture = nullptr;

	m_Device->CreateTexture2D(descriptor, subresourceData, &texture);
	if (Warning(!texture, "Failed to create texture")) {
		return nullptr;
	}

	m_Children.insert(texture);

	return texture;
}

kl::dx::Texture kl::GPU::newTexture(const Image& image, bool hasUnorderedAccess, bool isTarget) {
	Image flippedImage = image.flip(true);

	dx::TextureDesc descriptor = {};
	descriptor.Width = image.getWidth();
	descriptor.Height = image.getHeight();
	descriptor.MipLevels = 1;
	descriptor.ArraySize = 1;
	descriptor.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	descriptor.SampleDesc.Count = 1;
	descriptor.Usage = D3D11_USAGE_DEFAULT;
	descriptor.BindFlags = D3D11_BIND_SHADER_RESOURCE |
		(hasUnorderedAccess ? D3D11_BIND_UNORDERED_ACCESS : NULL) |
		(isTarget ? D3D11_BIND_RENDER_TARGET : NULL);

	dx::SubresDesc data = {};
	data.pSysMem = flippedImage.data();
	data.SysMemPitch = image.getWidth() * sizeof(Color);

	return newTexture(&descriptor, &data);
}

kl::dx::Texture kl::GPU::newTexture(const Image& front, const Image& back, const Image& left, const Image& right, const Image& top, const Image& bottom) {
	if (Warning(!(front.getSize() == back.getSize() &&
		front.getSize() == left.getSize() &&
		front.getSize() == right.getSize() &&
		front.getSize() == top.getSize() &&
		front.getSize() == bottom.getSize()),
		"Sizes of the 6 given images do not match")) {
		return nullptr;
	}

	dx::TextureDesc descriptor = {};
	descriptor.Width = front.getWidth();
	descriptor.Height = front.getHeight();
	descriptor.MipLevels = 1;
	descriptor.ArraySize = 6;
	descriptor.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	descriptor.SampleDesc.Count = 1;
	descriptor.Usage = D3D11_USAGE_DEFAULT;
	descriptor.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	descriptor.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	dx::SubresDesc data[6] = {
		{ right.data(), front.getWidth() * sizeof(Color), 0 },
		{ left.data(), front.getWidth() * sizeof(Color), 0 },
		{ top.data(), front.getWidth() * sizeof(Color), 0 },
		{ bottom.data(), front.getWidth() * sizeof(Color), 0 },
		{ front.data(), front.getWidth() * sizeof(Color), 0 },
		{ back.data(), front.getWidth() * sizeof(Color), 0 }
	};

	return newTexture(&descriptor, data);
}

kl::dx::Texture kl::GPU::newTextureStaging(dx::Texture texture, const UInt2& size) {
	dx::TextureDesc descriptor = {};
	texture->GetDesc(&descriptor);

	dx::TextureDesc stagingDescriptor = {};
	stagingDescriptor.Width = (size.x > 0) ? size.x : descriptor.Width;
	stagingDescriptor.Height = (size.y > 0) ? size.y : descriptor.Height;
	stagingDescriptor.MipLevels = 1;
	stagingDescriptor.ArraySize = 1;
	stagingDescriptor.Format = descriptor.Format;
	stagingDescriptor.SampleDesc.Count = 1;
	stagingDescriptor.Usage = D3D11_USAGE_STAGING;
	stagingDescriptor.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

	return newTexture(&stagingDescriptor);
}
