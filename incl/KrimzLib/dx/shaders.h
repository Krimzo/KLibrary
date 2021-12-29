#pragma once


namespace kl {
	namespace dx {
        struct shaders {
            shaders(ID3D11Device* dev, std::wstring shaderFile, std::string vertName, std::string pixlName) {
                // Vertex shader compilation
                D3DCompileFromFile(shaderFile.c_str(), nullptr, nullptr, vertName.c_str(), "vs_4_0", NULL, NULL, &vertBlob, nullptr);
                if (!vertBlob) exit(69);
                dev->CreateVertexShader(vertBlob->GetBufferPointer(), vertBlob->GetBufferSize(), NULL, &vertexShader);

                // Pixel shader compilation
                D3DCompileFromFile(shaderFile.c_str(), nullptr, nullptr, pixlName.c_str(), "ps_4_0", NULL, NULL, &pixlBlob, nullptr);
                if (!pixlBlob) exit(69);
                dev->CreatePixelShader(pixlBlob->GetBufferPointer(), pixlBlob->GetBufferSize(), NULL, &pixelShader);
            }
            ~shaders() {
                vertexShader->Release();
                pixelShader->Release();
                vertBlob->Release();
                pixlBlob->Release();
            }

            // Shader getters
            ID3D11VertexShader* getVert() {
                return vertexShader;
            }
            ID3D11PixelShader* getPixl() {
                return pixelShader;
            }

            // Shader blob getters
            ID3D10Blob* getVertBlob() {
                return vertBlob;
            }
            ID3D10Blob* getPixlBlob() {
                return pixlBlob;
            }

        private:
            ID3D10Blob* vertBlob = nullptr;
            ID3D10Blob* pixlBlob = nullptr;
            ID3D11VertexShader* vertexShader = nullptr;
            ID3D11PixelShader* pixelShader = nullptr;
        };
	}
}
