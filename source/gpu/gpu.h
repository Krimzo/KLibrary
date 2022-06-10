#pragma once

#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>

#include "render/vertex.h"
#include "graphics/image.h"

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
		ID3D11Device* m_Device = nullptr;
		ID3D11DeviceContext* m_Context = nullptr;
		IDXGISwapChain* m_Chain = nullptr;

		ID3D11RenderTargetView* m_FrameBuffer = nullptr;
		ID3D11DepthStencilView* m_DepthBuffer = nullptr;

		std::set<IUnknown*> m_Children;

		bool m_CBuffersPredefined = false;
		ID3D11Buffer* m_VertexCBuffers[KL_CBUFFER_PREDEFINED_SIZE] = {};
		ID3D11Buffer* m_PixelCBuffers[KL_CBUFFER_PREDEFINED_SIZE] = {};

	public:
		gpu(HWND hwnd, bool predefineCBuffers = true);
		gpu(const kl::gpu&) = delete;
		void operator=(const kl::gpu&) = delete;
		~gpu();

		ID3D11Device* dev();
		ID3D11DeviceContext* con();

		void viewport(const kl::int2& pos, const kl::uint2& size);

		void regenInternal(const kl::uint2& size);
		void bindInternal(const std::vector<ID3D11RenderTargetView*> targets = {}, ID3D11DepthStencilView* depthView = nullptr);
		void bindTargets(const std::vector<ID3D11RenderTargetView*> targets, ID3D11DepthStencilView* depthView = nullptr);

		void clearColor(const kl::float4& color);
		void clearDepth();
		void clear(const kl::float4& color);

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
		ID3D11VertexShader* newVertexShader(const std::string& source, ID3D11InputLayout** outLayout = nullptr, const std::vector<D3D11_INPUT_ELEMENT_DESC>& desc = {});
		ID3D11PixelShader* newPixelShader(const std::string& source);
		ID3D11GeometryShader* newGeometryShader(const std::string& source);
		ID3D11ComputeShader* newComputeShader(const std::string& source);
		kl::shaders newShaders(const std::string& vertSrc, const std::string& pixlSrc, const std::vector<D3D11_INPUT_ELEMENT_DESC>& desc = {});
		void bind(ID3D11VertexShader* sha);
		void bind(ID3D11PixelShader* sha);
		void bind(ID3D11GeometryShader* sha);
		void bind(ID3D11ComputeShader* sha);
		void bind(ID3D11InputLayout* layout);
		void bind(const kl::shaders& shaders, bool bindLayout = true);
		void dispatch(const kl::uint3& size);
		void execute(ID3D11ComputeShader* sha, const kl::uint3& size);

		// Buffer
		ID3D11Buffer* newBuffer(D3D11_BUFFER_DESC* desc, D3D11_SUBRESOURCE_DATA* subData = nullptr);

		// Constant buffer
		ID3D11Buffer* newCBuffer(uint byteSize);
		void setCBufferData(ID3D11Buffer* buff, const void* data);
		void bindVertexCBuffer(ID3D11Buffer* buff, uint slot);
		void bindPixelCBuffer(ID3D11Buffer* buff, uint slot);
		template<typename T> inline bool autoVertexCBuffer(const T& data, uint slot = 0) {
			if ((!m_CBuffersPredefined) || (sizeof(T) > (KL_CBUFFER_PREDEFINED_SIZE * 16)) || (sizeof(T) % 16)) {
				return false;
			}
			ID3D11Buffer* chosenBuffer = m_VertexCBuffers[(sizeof(T) / 16) - 1];
			bindVertexCBuffer(chosenBuffer, slot);
			setCBufferData(chosenBuffer, &data);
			return true;
		}
		template<typename T> inline bool autoPixelCBuffer(const T& data, uint slot = 0) {
			if ((!m_CBuffersPredefined) || (sizeof(T) > (KL_CBUFFER_PREDEFINED_SIZE * 16)) || (sizeof(T) % 16)) {
				return false;
			}
			ID3D11Buffer* chosenBuffer = m_PixelCBuffers[(sizeof(T) / 16) - 1];
			bindPixelCBuffer(chosenBuffer, slot);
			setCBufferData(chosenBuffer, &data);
			return true;
		}

		// Vertex buffer
		ID3D11Buffer* newVertexBuffer(const std::vector<kl::vertex>& vertexData);
		ID3D11Buffer* newVertexBuffer(const std::string& filePath, bool flipZ = true);
		void draw(ID3D11Buffer* mesh);

		// Sampler
		ID3D11SamplerState* newSamplerState(D3D11_SAMPLER_DESC* desc);
		ID3D11SamplerState* newSamplerState(bool linear, bool mirror);
		void bind(ID3D11SamplerState* sampState, uint slot);

		// Texture
		ID3D11Texture2D* newTextureBB();
		ID3D11Texture2D* newTexture(D3D11_TEXTURE2D_DESC* desc, D3D11_SUBRESOURCE_DATA* subData = nullptr);
		ID3D11Texture2D* newTexture(const kl::image& img, bool enableUnorderedAccess = false);
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
		void bindPixelShaderView(ID3D11ShaderResourceView* view, uint slot);
		void bindComputeShaderView(ID3D11ShaderResourceView* view, uint slot);

		// Shader access view
		ID3D11UnorderedAccessView* newAccessView(ID3D11Texture2D* tex, D3D11_UNORDERED_ACCESS_VIEW_DESC* desc = nullptr);
		void bindComputeAccessView(ID3D11UnorderedAccessView* view, uint slot, uint* initalCounts = nullptr);

		// Deletes child instance
		bool destroy(IUnknown* child);
	};
}
