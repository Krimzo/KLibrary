#pragma once

#include <iostream>
#include <vector>
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>

#include "KrimzLib/memory/pbuffer.h"
#include "KrimzLib/dx/gpu.h"
#include "KrimzLib/dx/fbuffer.h"
#include "KrimzLib/dx/raster.h"
#include "KrimzLib/dx/shaders.h"
#include "KrimzLib/dx/cbuffer.h"
#include "KrimzLib/dx/mesh.h"
#include "KrimzLib/dx/texture.h"
#include "KrimzLib/dx/sampler.h"
#include "KrimzLib/vecmath/ivec2.h"
#include "KrimzLib/vecmath/vec4.h"
#include "KrimzLib/geometry/vertex.h"
#include "KrimzLib/graphics/image.h"

#pragma comment (lib, "d3d11.lib")


namespace kl {
	class gpu {
	private:
		// DirectX pointers
		ID3D11Device* dev = nullptr;
		ID3D11DeviceContext* devcon = nullptr;
		IDXGISwapChain* chain = nullptr;
		kl::fbuffer* frameBuff = nullptr;

		// Buffers
		kl::pbuffer<kl::raster>   rasters;
		kl::pbuffer<kl::shaders>  shaders;
		kl::pbuffer<kl::cbuffer> cbuffers;
		kl::pbuffer<kl::mesh>      meshes;
		kl::pbuffer<kl::texture> textures;
		kl::pbuffer<kl::sampler> samplers;

	public:
		// Constructor
		gpu(HWND hwnd, int msaa = 2);

		// Destructor
		~gpu();

		// Getters
		ID3D11Device* getDev();
		ID3D11DeviceContext* getDevCon();

		// Sets the viewport
		void setViewport(const kl::ivec2& pos, const kl::ivec2& size);

		// Clears the buffer
		void clearColor(const kl::vec4& color);
		void clearDepth();
		void clear(const kl::vec4& color);

		// Swaps the buffers
		void swap(bool vSync);

		// Sets the depth testing state
		void setDepthTest(bool enabled);

		// Raster
		kl::raster* newRaster(bool wireframe, bool cull, bool cullBack = true);
		bool delRaster(kl::raster* ras);

		// Shaders
		kl::shaders* newShaders(const std::string& filePath, uint32_t vertBuffSize, uint32_t pixlBuffSize);
		bool delShaders(kl::shaders* sha);

		// Constant buffer
		kl::cbuffer* newCBuffer(int byteSize);
		bool delCBuffer(kl::cbuffer* cbuf);

		// Mesh
		kl::mesh* newMesh(const std::vector<kl::vertex>& vertexData);
		kl::mesh* newMesh(const std::string& filePath, bool flipZ);
		bool delMesh(kl::mesh* mes);

		// Texture
		kl::texture* newTexture(const kl::image& img);
		bool delTexture(kl::texture* tex);

		// Sampler
		kl::sampler* newSampler(bool linear, bool mirror);
		bool delSampler(kl::sampler* samp);
	};
}
