#pragma once

#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>

#include "libspec/using.h"
#include "math/int2.h"
#include "math/float4.h"
#include "geometry/vertex.h"
#include "graphics/image.h"

#pragma comment (lib, "d3d11.lib")

#define KL_CBUFFER_PREDEFINED_SIZE 64


namespace kl {
	struct shaders {
		ID3D11VertexShader* vertexS = nullptr;
		ID3D11PixelShader* pixelS = nullptr;
		ID3D11InputLayout* inLayout = nullptr;

		shaders() {}
		shaders(ID3D11VertexShader* vertexS, ID3D11PixelShader* pixelS, ID3D11InputLayout* inLayout) : vertexS(vertexS), pixelS(pixelS), inLayout(inLayout) {}
	};
}

namespace kl {
	class gpu {
	private:
		// DirectX pointers
		ID3D11Device* device = nullptr;
		ID3D11DeviceContext* devcon = nullptr;
		IDXGISwapChain* chain = nullptr;

		// Internal buffers
		ID3D11RenderTargetView* internalFrameBuffer = nullptr;
		ID3D11DepthStencilView* internalDepthBuffer = nullptr;

		// Buffers
		std::set<IUnknown*> children;

		// Predefined constant buffers
		bool cbuffersPredefined = false;
		ID3D11Buffer* vertexCBuffers[KL_CBUFFER_PREDEFINED_SIZE] = {};
		ID3D11Buffer* pixelCBuffers[KL_CBUFFER_PREDEFINED_SIZE] = {};

	public:
		gpu(HWND hwnd, bool predefineCBuffers = true);
		gpu(const kl::gpu&) = delete;
		void operator=(const kl::gpu&) = delete;
		~gpu();

		// Getters
		ID3D11Device* dev();
		ID3D11DeviceContext* con();

		// Sets the viewport
		void viewport(const kl::int2& pos, const kl::int2& size);

		// Internal buffers
		void regenInternal(const kl::int2& size);
		void bindInternal(const std::vector<ID3D11RenderTargetView*> targets = {}, ID3D11DepthStencilView* depthView = nullptr);

		// Binds given render target
		void bindTargets(const std::vector<ID3D11RenderTargetView*> targets, ID3D11DepthStencilView* depthView = nullptr);

		// Clears the buffer
		void clearColor(const kl::float4& color);
		void clearDepth();
		void clear(const kl::float4& color);

		// Swaps the buffers
		void swap(bool vSync);

		// Raster state
		ID3D11RasterizerState* newRasterState(D3D11_RASTERIZER_DESC* desc);
		ID3D11RasterizerState* newRasterState(bool wireframe, bool cull, bool cullBack = true);
		void bind(ID3D11RasterizerState* state);

		// Depth stenicl state
		ID3D11DepthStencilState* newDepthState(D3D11_DEPTH_STENCIL_DESC* desc);
		ID3D11DepthStencilState* newDepthState(bool depth, bool stencil, bool mask);
		void bind(ID3D11DepthStencilState* state);

		// Shaders
		ID3D11VertexShader* newVertexShader(const String& source, ID3D11InputLayout** outLayout = nullptr, const std::vector<D3D11_INPUT_ELEMENT_DESC>& desc = {});
		ID3D11PixelShader* newPixelShader(const String& source);
		ID3D11GeometryShader* newGeometryShader(const String& source);
		ID3D11ComputeShader* newComputeShader(const String& source);
		kl::shaders newShaders(const String& vertSrc, const String& pixlSrc, const std::vector<D3D11_INPUT_ELEMENT_DESC>& desc = {});
		void bind(ID3D11VertexShader* sha);
		void bind(ID3D11PixelShader* sha);
		void bind(ID3D11GeometryShader* sha);
		void bind(ID3D11ComputeShader* sha);
		void bind(ID3D11InputLayout* layout);
		void bind(const kl::shaders& shaders, bool bindLayout = true);
		void dispatch(const kl::int3& size);
		void execute(ID3D11ComputeShader* sha, const kl::int3& size);

		// Buffer
		ID3D11Buffer* newBuffer(D3D11_BUFFER_DESC* desc, D3D11_SUBRESOURCE_DATA* subData = nullptr);

		// Constant buffer
		ID3D11Buffer* newCBuffer(uint32_t byteSize);
		void setCBufferData(ID3D11Buffer* buff, const void* data);
		void bindVertexCBuffer(ID3D11Buffer* buff, uint32_t slot);
		void bindPixelCBuffer(ID3D11Buffer* buff, uint32_t slot);
		template<typename T> inline bool autoVertexCBuffer(const T& data, uint32_t slot = 0) {
			if ((!cbuffersPredefined) || (sizeof(T) > (KL_CBUFFER_PREDEFINED_SIZE * 16)) || (sizeof(T) % 16)) {
				return false;
			}
			ID3D11Buffer* chosenBuffer = vertexCBuffers[(sizeof(T) / 16) - 1];
			bindVertexCBuffer(chosenBuffer, slot);
			setCBufferData(chosenBuffer, &data);
			return true;
		}
		template<typename T> inline bool autoPixelCBuffer(const T& data, uint32_t slot = 0) {
			if ((!cbuffersPredefined) || (sizeof(T) > (KL_CBUFFER_PREDEFINED_SIZE * 16)) || (sizeof(T) % 16)) {
				return false;
			}
			ID3D11Buffer* chosenBuffer = pixelCBuffers[(sizeof(T) / 16) - 1];
			bindPixelCBuffer(chosenBuffer, slot);
			setCBufferData(chosenBuffer, &data);
			return true;
		}

		// Mesh
		ID3D11Buffer* newVertexBuffer(const std::vector<kl::vertex>& vertexData);
		ID3D11Buffer* newVertexBuffer(const String& filePath, bool flipZ = true);
		void draw(ID3D11Buffer* mesh);

		// Sampler
		ID3D11SamplerState* newSamplerState(D3D11_SAMPLER_DESC* desc);
		ID3D11SamplerState* newSamplerState(bool linear, bool mirror);
		void bind(ID3D11SamplerState* sampState, uint32_t slot);

		// Texture
		ID3D11Texture2D* newTextureBB();
		ID3D11Texture2D* newTexture(D3D11_TEXTURE2D_DESC* desc, D3D11_SUBRESOURCE_DATA* subData = nullptr);
		ID3D11Texture2D* newTexture(const kl::image& img);
		ID3D11Texture2D* newTexture(const kl::image& front, const kl::image& back, const kl::image& left, const kl::image& right, const kl::image& top, const kl::image& bottom);
		ID3D11Texture2D* newTextureST(ID3D11Texture2D* tex, const kl::int2& size = {});

		// Render target view
		ID3D11RenderTargetView* newTargetView(ID3D11Texture2D* tex, D3D11_RENDER_TARGET_VIEW_DESC* desc = nullptr);
		void clear(ID3D11RenderTargetView* view, const kl::float4& color);

		// Depth stencil view
		ID3D11DepthStencilView* newDepthView(ID3D11Texture2D* tex, D3D11_DEPTH_STENCIL_VIEW_DESC* desc = nullptr);
		void clear(ID3D11DepthStencilView* view, float depth = 1.0f, byte stencil = 0);

		// Shader resource view
		ID3D11ShaderResourceView* newShaderView(ID3D11Texture2D* tex, D3D11_SHADER_RESOURCE_VIEW_DESC* desc = nullptr);
		void bindPixlTex(ID3D11ShaderResourceView* buff, uint32_t slot);

		// Deletes child instance
		bool destroy(IUnknown* child);
	};
}
