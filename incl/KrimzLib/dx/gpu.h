#pragma once


namespace kl {
	namespace dx {
        struct gpu {
            // Constructor/destructor
            gpu(kl::window& win, int AA = 4) {
                // Device and swapchain creation
                DXGI_SWAP_CHAIN_DESC scd = {};
                scd.BufferCount = 1;
                scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
                scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
                scd.OutputWindow = win.getHWND();
                scd.SampleDesc.Count = AA;
                scd.Windowed = true;
                D3D11CreateDeviceAndSwapChain(
                    nullptr,
                    D3D_DRIVER_TYPE_HARDWARE,
                    nullptr,
                    NULL,
                    nullptr,
                    NULL,
                    D3D11_SDK_VERSION,
                    &scd,
                    &swapchain,
                    &dev,
                    nullptr,
                    &devcon
                );
                if (!dev || !devcon || !swapchain) exit(69);

                // Getting the backbuffer address
                ID3D11Texture2D* backbufferAddr = nullptr;
                swapchain->GetBuffer(NULL, __uuidof(ID3D11Texture2D), (void**)&backbufferAddr);
                if (!backbufferAddr) exit(69);

                // Creating the backbuffer
                dev->CreateRenderTargetView(backbufferAddr, nullptr, &backbuffer);
                if (!backbuffer) exit(69);

                // Setting the render target
                devcon->OMSetRenderTargets(1, &backbuffer, nullptr);
                backbufferAddr->Release();

                // Setting up the viewport
                D3D11_VIEWPORT viewport = {};
                viewport.TopLeftX = 0;
                viewport.TopLeftY = 0;
                viewport.Width = win.getSize().x;
                viewport.Height = win.getSize().y;
                viewport.MinDepth = 0;
                viewport.MaxDepth = 1;
                devcon->RSSetViewports(1, &viewport);

                // Selecting the triangles as the render type
                devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
            }
            ~gpu() {
                backbuffer->Release();
                swapchain->Release();
                devcon->Release();
                dev->Release();
            }

            // Clears the frame buffers
            void clear(kl::vec3 col) {
                float pCol[4] = { col.x, col.y, col.z, 1 };
                devcon->ClearRenderTargetView(backbuffer, pCol);
            }

            // Draws the triangles
            void render(int vertCount) {
                devcon->Draw(vertCount, 0);
            }

            // Swaps the frame buffers
            void swap() {
                swapchain->Present(0, NULL);
            }

            // Compiles and returns shaders
            kl::dx::shaders* newShaders(std::string filePath, std::string vertName, std::string pixlName) {
                return new kl::dx::shaders(dev, kl::convert::toWString(filePath), vertName, pixlName);
            }

            // Binds the shaders to the gpu
            void bindShaders(kl::dx::shaders* sha) {
                devcon->VSSetShader(sha->getVert(), 0, 0);
                devcon->PSSetShader(sha->getPixl(), 0, 0);
                vertBlob = sha->getVertBlob();
                pixlBlob = sha->getPixlBlob();
            }

            // Creates and returns a new buffer
            kl::dx::buffer* newBuffer(void* data, int byteSize, D3D11_USAGE usage) {
                return new kl::dx::buffer(dev, data, byteSize, usage);
            }

            // Binds a buffer as a vertex buffer
            void bindVertBuff(kl::dx::buffer* buff, int stride, int offset) {
                UINT tempStride = stride;
                UINT tempOffset = offset;
                ID3D11Buffer* tempBuff = buff->getBuff();
                devcon->IASetVertexBuffers(0, 1, &tempBuff, &tempStride, &tempOffset);
            }

            // Sets the buffer layout
            void setLayouts(std::vector<kl::dx::layout>& layouts) {
                // Input descriptor buffer
                D3D11_INPUT_ELEMENT_DESC* inputDescriptor = new D3D11_INPUT_ELEMENT_DESC[layouts.size()];

                // Setting up descriptor buffer
                for (int i = 0; i < layouts.size(); i++) {
                    inputDescriptor[i].SemanticName = layouts[i].name.c_str();
                    inputDescriptor[i].SemanticIndex = 0;
                    inputDescriptor[i].Format = layouts[i].floats == 3 ? DXGI_FORMAT_R32G32B32_FLOAT : (layouts[i].floats == 2 ? DXGI_FORMAT_R32G32_FLOAT : DXGI_FORMAT_R32_FLOAT);
                    inputDescriptor[i].InputSlot = 0;
                    inputDescriptor[i].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
                    inputDescriptor[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
                    inputDescriptor[i].InstanceDataStepRate = 0;
                }

                // Setting the buffer layout
                ID3D11InputLayout* bufferLayout = nullptr;
                dev->CreateInputLayout(inputDescriptor, 3, vertBlob->GetBufferPointer(), vertBlob->GetBufferSize(), &bufferLayout);
                if (!bufferLayout) exit(69);
                devcon->IASetInputLayout(bufferLayout);
            }

        private:
            // Device pointers
            ID3D11Device* dev = nullptr;
            ID3D11DeviceContext* devcon = nullptr;
            IDXGISwapChain* swapchain = nullptr;
            ID3D11RenderTargetView* backbuffer = nullptr;

            // Shader pointers
            ID3D10Blob* vertBlob = nullptr;
            ID3D10Blob* pixlBlob = nullptr;
        };
	}
}
