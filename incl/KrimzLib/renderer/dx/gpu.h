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
                return new kl::dx::shaders(dev, devcon, kl::convert::toWString(filePath), vertName, pixlName);
            }

            // Binds the shaders to the gpu
            void bindShaders(kl::dx::shaders* sha) {
                devcon->VSSetShader(sha->getVert(), 0, 0);
                devcon->PSSetShader(sha->getPixl(), 0, 0);
                vertBlob = sha->getVertBlob();
                pixlBlob = sha->getPixlBlob();
            }

            // Creates and returns a new mesh
            kl::dx::mesh* newMesh(std::vector<kl::vertex>& vertexData) {
                return new kl::dx::mesh(dev, vertexData);
            }
            kl::dx::mesh* newMesh(std::string filePath, bool flipZ) {
                return new kl::dx::mesh(dev, filePath, flipZ);
            }

            // Binds a buffer as a vertex buffer
            void bindMesh(kl::dx::mesh* mesh) {
                UINT tempStride = sizeof(kl::vertex);
                UINT tempOffset = 0;
                ID3D11Buffer* tempBuff = mesh->getBuff();
                devcon->IASetVertexBuffers(0, 1, &tempBuff, &tempStride, &tempOffset);
            }

            // Creates and returns a new constant buffer
            kl::dx::cbuffer* newCBuffer(int byteSize) {
                return new kl::dx::cbuffer(dev, devcon, byteSize);
            }

            // Binds a buffer as a constant buffer
            void bindCBuff(kl::dx::cbuffer* cbuff) {
                ID3D11Buffer* tempCBuff = cbuff->getBuff();
                devcon->VSSetConstantBuffers(0, 1, &tempCBuff);
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
