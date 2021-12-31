#pragma once


namespace kl {
	namespace dx {
		struct texture {
			// Constructors/destructor
			texture(ID3D11Device* dev, kl::image& image) {
				load2D(dev, image);
			}
			~texture() {
				view->Release();
			}

			// Returns a pointer to the texture view
			ID3D11ShaderResourceView* getView() {
				return view;
			}

		private:
			ID3D11ShaderResourceView* view = nullptr;

			// Loads the image data to a texture view
			void load2D(ID3D11Device* dev, kl::image& image) {
				// Getting the image pixel data pointer
				const kl::color* imagePointer = (kl::color*)image.pointer();

				// Allocating the temp pixel memory
				unsigned int* pixelData = new unsigned int[image.getWidth() * image.getHeight()];
				kl::console::error(!pixelData, "Memory: Could not allocated temp pixel data!");

				// Flipping the image on y
				image.flipVertical();

				// Reading and storing the pixel data
				for (int i = 0; i < image.getWidth() * image.getHeight(); i++) {
					pixelData[i] = 0xFF;
					pixelData[i] = (pixelData[i] << 8) + int(imagePointer[i].b);
					pixelData[i] = (pixelData[i] << 8) + int(imagePointer[i].g);
					pixelData[i] = (pixelData[i] << 8) + int(imagePointer[i].r);
				}

				// Resetting the flip
				image.flipVertical();

				// Creating the texture resource
				D3D11_TEXTURE2D_DESC texDesc = {};
				texDesc.Width = image.getWidth();
				texDesc.Height = image.getHeight();
				texDesc.MipLevels = 1;
				texDesc.ArraySize = 1;
				texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
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
				kl::console::error(!tex, "DirectX: Could not create a 2D texture!");

				// Deleting temp pixel data
				delete pixelData;

				// Creating a resource view on the texture and cleanup
				D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc = {};
				viewDesc.Format = texDesc.Format;
				viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				viewDesc.Texture2D.MipLevels = 1;
				viewDesc.Texture2D.MostDetailedMip = 0;
				dev->CreateShaderResourceView(tex, &viewDesc, &view);
				kl::console::error(!view, "DirectX: Could not create a 2D texture view!");
				tex->Release();
			}
		};
	}
}
