#pragma once


namespace kl {
	namespace dx {
        struct shaders {
            shaders(ID3D11Device* dev, ID3D11DeviceContext* devcon, std::wstring shaderFile, std::string vertName, std::string pixlName) {
                // Vertex shader compilation
                D3DCompileFromFile(shaderFile.c_str(), nullptr, nullptr, vertName.c_str(), "vs_4_0", NULL, NULL, &vertBlob, nullptr);
                if (!vertBlob) kl::console::error("DirectX: Could not compile a vertex shader!");
                dev->CreateVertexShader(vertBlob->GetBufferPointer(), vertBlob->GetBufferSize(), NULL, &vertexShader);

                // Pixel shader compilation
                D3DCompileFromFile(shaderFile.c_str(), nullptr, nullptr, pixlName.c_str(), "ps_4_0", NULL, NULL, &pixlBlob, nullptr);
                if (!pixlBlob) kl::console::error("DirectX: Could not compile a pixel shader!");
                dev->CreatePixelShader(pixlBlob->GetBufferPointer(), pixlBlob->GetBufferSize(), NULL, &pixelShader);

                // Input descriptor buffer
                D3D11_INPUT_ELEMENT_DESC inputDescriptor[3] = {
                    {  "POSIN", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                    {  "TEXIN", 0,    DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                    { "NORMIN", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
                };

                // Setting the buffer layout
                ID3D11InputLayout* bufferLayout = nullptr;
                dev->CreateInputLayout(inputDescriptor, 3, vertBlob->GetBufferPointer(), vertBlob->GetBufferSize(), &bufferLayout);
                if (!bufferLayout) kl::console::error("DirectX: Could not create an input layout!");
                devcon->IASetInputLayout(bufferLayout);
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
