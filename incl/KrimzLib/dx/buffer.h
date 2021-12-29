#pragma once


namespace kl {
	namespace dx {
        struct layout {
            std::string name;
            int floats;

            // Constructor
            layout(std::string name, int floats) {
                this->name = name;
                this->floats = floats;
            }
        };

        struct buffer {
            buffer(ID3D11Device* dev, void* data, int byteSize, D3D11_USAGE usage) {
                D3D11_BUFFER_DESC bufferDescriptor = {};
                bufferDescriptor.Usage = usage;
                bufferDescriptor.ByteWidth = byteSize;
                bufferDescriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;
                bufferDescriptor.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
                D3D11_SUBRESOURCE_DATA bufferData = {};
                bufferData.pSysMem = data;
                dev->CreateBuffer(&bufferDescriptor, &bufferData, &buff);
                if (!buff) exit(69);
            }
            ~buffer() {
                buff->Release();
            }

            // Buffer getter
            ID3D11Buffer* getBuff() {
                return buff;
            }

        private:
            ID3D11Buffer* buff = nullptr;
        };
	}
}
