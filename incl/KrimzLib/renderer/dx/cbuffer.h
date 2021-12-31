#pragma once


namespace kl {
	namespace dx {
		struct cbuffer {
			// Constructor/destructor
			cbuffer(ID3D11Device* dev, ID3D11DeviceContext* devcon, int byteSize) {
                // Buffer descriptor setting
                D3D11_BUFFER_DESC bufferDescriptor = {};
                bufferDescriptor.ByteWidth = byteSize;
                bufferDescriptor.Usage = D3D11_USAGE_DYNAMIC;
                bufferDescriptor.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
                bufferDescriptor.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
                bufferDescriptor.MiscFlags = NULL;
                bufferDescriptor.StructureByteStride = 0;

                // Buffer creation
                dev->CreateBuffer(&bufferDescriptor, nullptr, &buff);
                kl::console::error(!buff, "DirectX: Could not create a constant buffer!");

                // Saving the devcon for later data setting
                this->devcon = devcon;

                // Saving the buffer size
                this->buffSize = byteSize;
			}
			~cbuffer() {
                buff->Release();
			}

            // Buffer getter
            ID3D11Buffer* getBuff() {
                return buff;
            }

            // Sets the data of the buffer
            void setData(void* data) {
                D3D11_MAPPED_SUBRESOURCE ms;
                devcon->Map(buff, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
                memcpy(ms.pData, data, buffSize);
                devcon->Unmap(buff, NULL);
            }

		private:
            ID3D11Buffer* buff = nullptr;
            ID3D11DeviceContext* devcon = nullptr;
            int buffSize = 0;
		};
	}
}
