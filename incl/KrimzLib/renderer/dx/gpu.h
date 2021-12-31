#pragma once


namespace kl {
	namespace dx {
        struct gpu {
            // Constructor/destructor
            gpu(kl::window* win, int AA = 1) {
                // Device and swapchain creation
                newSwapChain(win->getHWND(), AA);

                // Setting up the viewport
                newViewport(win->getSize());

                // Setting a new raster state
                setRaster(false);
                
                // Creating buffers
                newBuffers(win->getSize(), AA);

                // Selecting the triangles as the render type
                devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
            }
            ~gpu() {
                backbuffer->Release();
                swapchain->Release();
                devcon->Release();
                dev->Release();
            }

            // Sets a new rasterizer state
            void setRaster(bool wireframe) {
                // Creating a raster state
                D3D11_RASTERIZER_DESC rasterStateDesc = {};
                rasterStateDesc.FillMode = wireframe ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID;
                rasterStateDesc.CullMode = D3D11_CULL_NONE;
                rasterStateDesc.FrontCounterClockwise = true;
                rasterStateDesc.MultisampleEnable = true;
                rasterStateDesc.AntialiasedLineEnable = true;
                ID3D11RasterizerState* rasterState = nullptr;
                dev->CreateRasterizerState(&rasterStateDesc, &rasterState);

                // Setting the raster state and cleanup
                devcon->RSSetState(rasterState);
                rasterState->Release();
            }

            // Clears the frame buffers
            void clear(kl::vec3 col) {
                float pCol[4] = { col.x, col.y, col.z, 1 };
                devcon->ClearRenderTargetView(backbuffer, pCol);
                devcon->ClearDepthStencilView(depthbuffer, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
            }

            // Draws the triangles
            void render(int vertCount) {
                devcon->Draw(vertCount, 0);
            }

            // Swaps the frame buffers
            void swap(bool vSync) {
                swapchain->Present(vSync, NULL);
            }

            // Compiles and returns shaders
            kl::dx::shaders* newShaders(std::string filePath, std::string vertName, std::string pixlName) {
                return new kl::dx::shaders(dev, devcon, kl::convert::toWString(filePath), vertName, pixlName);
            }

            // Binds the shaders to the gpu
            void bindShaders(kl::dx::shaders* sha) {
                devcon->VSSetShader(sha->getVert(), nullptr, 0);
                devcon->PSSetShader(sha->getPixl(), nullptr, 0);
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

            // Creates a new texture sampler
            kl::dx::sampler* newSampler() {
                return new kl::dx::sampler(dev);
            }
            
            // Binds a pixel shader texture sampler
            void bindSampler(kl::dx::sampler* samp, int slot) {
                ID3D11SamplerState* tempState = samp->getState();
                devcon->PSSetSamplers(slot, 1, &tempState);
            }

            // Creates and returns a new texture
            kl::dx::texture* newTexture(kl::image& image) {
                return new kl::dx::texture(dev, image);
            }
            kl::dx::texture* newTexture(kl::image&& image) {
                return new kl::dx::texture(dev, image);
            }

            // Binds a texture to the pixel shader
            void bindTexture(kl::dx::texture* tex, int slot) {
                // Binding the texture view
                ID3D11ShaderResourceView* tempView = tex->getView();
                devcon->PSSetShaderResources(slot, 1, &tempView);
            }

            // Creates and returns a new constant buffer
            kl::dx::cbuffer* newCBuffer(int byteSize) {
                return new kl::dx::cbuffer(dev, devcon, byteSize);
            }

            // Binds a buffer as a constant buffer
            void bindCBuff(kl::dx::cbuffer* cbuff, int slot) {
                ID3D11Buffer* tempCBuff = cbuff->getBuff();
                devcon->VSSetConstantBuffers(slot, 1, &tempCBuff);
            }

        private:
            // Device pointers
            ID3D11Device* dev = nullptr;
            ID3D11DeviceContext* devcon = nullptr;
            IDXGISwapChain* swapchain = nullptr;
            ID3D11RenderTargetView* backbuffer = nullptr;
            ID3D11DepthStencilView* depthbuffer = nullptr;

            // Creates a new device and swapchain
            void newSwapChain(HWND win, unsigned int AA) {
                // Swapchain data buffer
                DXGI_SWAP_CHAIN_DESC scd = {};
                scd.BufferCount = 1;
                scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
                scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
                scd.OutputWindow = win;
                scd.SampleDesc.Count = AA;
                scd.Windowed = true;

                // Creating the device and sc
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

                // Error checking
                kl::console::error(!dev, "DirectX: Could not create a device!");
                kl::console::error(!devcon, "DirectX: Could not create a device context!");
                kl::console::error(!swapchain, "DirectX: Could not create a swapchain!");
            }

            // Sets the gpu viewport
            void newViewport(kl::ivec2 size) {
                D3D11_VIEWPORT viewport = {};
                viewport.TopLeftX = 0;
                viewport.TopLeftY = 0;
                viewport.Width = size.x;
                viewport.Height = size.y;
                viewport.MinDepth = 0;
                viewport.MaxDepth = 1;
                devcon->RSSetViewports(1, &viewport);
            }

            // Creates and sets the frame, depth and stencil buffers
            void newBuffers(kl::ivec2 size, int AA) {
                // Getting the backbuffer address
                ID3D11Texture2D* backbufferAddr = nullptr;
                swapchain->GetBuffer(NULL, __uuidof(ID3D11Texture2D), (void**)&backbufferAddr);
                kl::console::error(!backbufferAddr, "DirectX: Could not get a backbuffer address!");

                // Creating the backbuffer and cleanup
                dev->CreateRenderTargetView(backbufferAddr, nullptr, &backbuffer);
                kl::console::error(!backbuffer, "DirectX: Could not create a render target view!");
                backbufferAddr->Release();

                // Creating depth/stencil state
                D3D11_DEPTH_STENCIL_DESC depthDesc = {};
                depthDesc.DepthEnable = true;
                depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
                depthDesc.DepthFunc = D3D11_COMPARISON_LESS;
                depthDesc.StencilEnable = false;
                ID3D11DepthStencilState* depthState = nullptr;
                dev->CreateDepthStencilState(&depthDesc, &depthState);
                kl::console::error(!depthState, "DirectX: Could not create a depth/stencil state!");

                // Setting the depth/stencil state and cleanup
                devcon->OMSetDepthStencilState(depthState, 1);
                depthState->Release();

                // Creating depth/stencil textures
                D3D11_TEXTURE2D_DESC depthTexDesc = {};
                depthTexDesc.Width = size.x;
                depthTexDesc.Height = size.y;
                depthTexDesc.MipLevels = 1;
                depthTexDesc.ArraySize = 1;
                depthTexDesc.Format = DXGI_FORMAT_D32_FLOAT;
                depthTexDesc.SampleDesc.Count = AA;
                depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
                depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
                ID3D11Texture2D* depthTex = nullptr;
                dev->CreateTexture2D(&depthTexDesc, NULL, &depthTex);
                kl::console::error(!depthTex, "DirectX: Could not create a depth/stencil buffer texture!");

                // Creating depth/stencil buffers and cleanup
                D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
                dsvDesc.Format = depthTexDesc.Format;
                dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
                dev->CreateDepthStencilView(depthTex, &dsvDesc, &depthbuffer);
                kl::console::error(!depthbuffer, "DirectX: Could not create a depth/stencil buffer view!");
                depthTex->Release();

                // Setting the render targets
                devcon->OMSetRenderTargets(1, &backbuffer, depthbuffer);
            }
        };
	}
}
