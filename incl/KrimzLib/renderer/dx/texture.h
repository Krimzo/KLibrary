#pragma once


namespace kl {
	namespace dx {
		struct texture {
			// Constructors/destructor
			texture(ID3D11Device* dev, kl::image& image) {
				newSampler(dev);
				load2D(dev, image);
			}
			~texture() {
				sampState->Release();
				texView->Release();
			}

			// Returns a pointer to the sampler state
			ID3D11SamplerState* getSampler() {
				return sampState;
			}

			// Returns a pointer to the texture view
			ID3D11ShaderResourceView* getView() {
				return texView;
			}

		private:
			ID3D11SamplerState* sampState = nullptr;
			ID3D11ShaderResourceView* texView = nullptr;

			// Creating a new texture sampler
			void newSampler(ID3D11Device* dev) {
				D3D11_SAMPLER_DESC sampDesc = {};
				sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
				sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
				sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
				sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
				dev->CreateSamplerState(&sampDesc, &sampState);
				if (!sampState) kl::console::error("DirectX: Could not create a sampler state!");
			}

			// Loads the image data to a texture view
			void load2D(ID3D11Device* dev, kl::image& image) {
				// Pixel data conversion
				const kl::color* pixelPointer = (kl::color*)image.pointer();
				const int pixelCount = image.getWidth() * image.getHeight();
				unsigned int* pixelData = new unsigned int[pixelCount];
				if (!pixelData) kl::console::error("Memory: Could not allocated temp pixel data!");
				for (int i = 0; i < pixelCount; i++) {
					pixelData[i] = int(pixelPointer[i].r);
					pixelData[i] = (pixelData[i] << 8) + int(pixelPointer[i].g);
					pixelData[i] = (pixelData[i] << 8) + int(pixelPointer[i].b);
					pixelData[i] = (pixelData[i] << 8) + 0xFF;
				}

				// Texture creation
				D3D11_TEXTURE2D_DESC texDesc = {};
				texDesc.Width = image.getWidth();
				texDesc.Height = image.getHeight();
				texDesc.MipLevels = 1;
				texDesc.ArraySize = 1;
				texDesc.Format = DXGI_FORMAT_R8G8B8A8_UINT;
				texDesc.SampleDesc.Count = 1;
				texDesc.Usage = D3D11_USAGE_DEFAULT;
				texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
				texDesc.CPUAccessFlags = NULL;
				texDesc.MiscFlags = NULL;
				D3D11_SUBRESOURCE_DATA texData = {};
				texData.pSysMem = pixelData;
				texData.SysMemPitch = image.getWidth() * sizeof(unsigned int);
				ID3D11Texture2D* tex = nullptr;
				dev->CreateTexture2D(&texDesc, &texData, &tex);
				if (!tex) kl::console::error("DirectX: Could not create a 2D texture!");

				// Creating a resource view on the texture
				D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc = {};
				viewDesc.Format = texDesc.Format;
				viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				viewDesc.Texture2D.MostDetailedMip = 0;
				viewDesc.Texture2D.MipLevels = 1;
				dev->CreateShaderResourceView(tex, &viewDesc, &texView);
				if (!texView) kl::console::error("DirectX: Could not create a 2D texture view!");

				// Deleting temp pixel data
				delete pixelData;
			}
		};
	}
}
