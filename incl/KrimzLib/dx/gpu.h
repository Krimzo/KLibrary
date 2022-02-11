#pragma once

#include <iostream>
#include <vector>
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>

#include "KrimzLib/dx/gpu.h"
#include "KrimzLib/dx/raster.h"
#include "KrimzLib/dx/shaders.h"
#include "KrimzLib/dx/cbuffer.h"
#include "KrimzLib/dx/mesh.h"
#include "KrimzLib/dx/texture.h"
#include "KrimzLib/dx/sampler.h"
#include "KrimzLib/vecmath/ivec2.h"
#include "KrimzLib/vecmath/vec4.h"
#include "KrimzLib/renderer/vertex.h"
#include "KrimzLib/graphics/image.h"

#pragma comment (lib, "d3d11.lib")


namespace kl {
	class gpu {
	private:
		// DirectX pointers
		IDXGISwapChain* chain = nullptr;
		ID3D11Device* dev = nullptr;
		ID3D11DeviceContext* devcon = nullptr;
		ID3D11RenderTargetView* backBuff = nullptr;
		ID3D11DepthStencilView* depthBuff = nullptr;

		// MSAA
		int msaa = 0;

	public:
		// Constructor
		gpu(HWND hwnd, int msaa = 2);

		// Destructor
		~gpu();

		// Sets the viewport
		void setViewport(const kl::ivec2& pos, const kl::ivec2& size);

		// Clears the buffer
		void clear(const kl::vec4& color);

		// Swaps the buffers
		void swap(bool vSync);

		// Raster
		kl::raster* newRaster(bool wireframe, bool cull, bool cullBack);

		// Shaders
		kl::shaders* newShaders(const std::string& filePath);

		// Constant buffer
		kl::cbuffer* newCBuffer(int byteSize);

		// Mesh
		kl::mesh* newMesh(const std::vector<kl::vertex>& vertexData);
		kl::mesh* newMesh(const std::string& filePath, bool flipZ);

		// Texture
		kl::texture* newTexture(const kl::image& img);

		// Sampler
		kl::sampler* newSampler(bool linear, bool mirror);
	};
}
