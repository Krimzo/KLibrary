#pragma once

#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>

#include "gpu/dx_types.h"
#include "render/vertex.h"
#include "graphics/image.h"


inline constexpr uint KL_CBUFFER_PREDEFINED_SIZE = 64;

namespace kl {
	template<typename T> using reference = std::shared_ptr<T>;
	template<typename T, typename... Args> inline kl::reference<T> make(const Args&... args) {
		return std::make_shared<T>(args...);
	}
}

namespace kl {
	struct shaders {
		kl::dx::shader::vertex vertexS = nullptr;
		kl::dx::shader::pixel pixelS = nullptr;
		kl::dx::shader::input inLayout = nullptr;

		shaders() {}
		shaders(kl::dx::shader::vertex vertexS, kl::dx::shader::pixel pixelS, kl::dx::shader::input inLayout) : vertexS(vertexS), pixelS(pixelS), inLayout(inLayout) {}
	};
}

namespace kl {
	class gpu {
	private:
		kl::dx::device m_Device = nullptr;
		kl::dx::context m_Context = nullptr;
		kl::dx::chain m_Chain = nullptr;

		kl::dx::view::target m_FrameBuffer = nullptr;
		kl::dx::view::depth m_DepthBuffer = nullptr;

		std::set<IUnknown*> m_Children;

		bool m_CBuffersPredefined = false;
		kl::dx::buffer m_VertexCBuffers[KL_CBUFFER_PREDEFINED_SIZE] = {};
		kl::dx::buffer m_PixelCBuffers[KL_CBUFFER_PREDEFINED_SIZE] = {};

	public:
		gpu(HWND hwnd, bool predefineCBuffers = true);
		gpu(const kl::gpu&) = delete;
		void operator=(const kl::gpu&) = delete;
		~gpu();

		kl::dx::device dev();
		kl::dx::context con();

		void viewport(const kl::uint2& size);
		void viewport(const kl::int2& pos, const kl::uint2& size);

		void regenInternal(const kl::uint2& size);
		void bindInternal(const std::vector<kl::dx::view::target> targets = {}, kl::dx::view::depth depthView = nullptr);
		void bindTargets(const std::vector<kl::dx::view::target> targets, kl::dx::view::depth depthView = nullptr);

		void clearColor(const kl::float4& color);
		void clearDepth();
		void clear(const kl::float4& color);

		void swap(bool vSync);

		// Raster state
		kl::dx::state::raster newRasterState(kl::dx::state::desc::raster* desc);
		kl::dx::state::raster newRasterState(bool wireframe, bool cull, bool cullBack = true);
		void bind(kl::dx::state::raster state);

		// Depth stenicl state
		kl::dx::state::depth newDepthState(kl::dx::state::desc::depth* desc);
		kl::dx::state::depth newDepthState(bool depth, bool stencil, bool mask);
		void bind(kl::dx::state::depth state);

		// Shaders
		kl::dx::shader::vertex newVertexShader(const std::string& source, kl::dx::shader::input* outLayout = nullptr, const std::vector<kl::dx::shader::desc::input>& desc = {});
		kl::dx::shader::pixel newPixelShader(const std::string& source);
		kl::dx::shader::geometry newGeometryShader(const std::string& source);
		kl::dx::shader::compute newComputeShader(const std::string& source);
		kl::shaders newShaders(const std::string& vertSrc, const std::string& pixlSrc, const std::vector<kl::dx::shader::desc::input>& desc = {});
		kl::shaders newShaders(const std::string& fullSrc, const std::vector<kl::dx::shader::desc::input>& desc = {});
		void bind(kl::dx::shader::vertex sha);
		void bind(kl::dx::shader::pixel sha);
		void bind(kl::dx::shader::geometry sha);
		void bind(kl::dx::shader::compute sha);
		void bind(kl::dx::shader::input layout);
		void bind(const kl::shaders& shaders, bool bindLayout = true);
		void dispatch(const kl::uint3& size);
		void execute(kl::dx::shader::compute sha, const kl::uint3& size);

		// Buffer
		kl::dx::buffer newBuffer(kl::dx::desc::buffer* desc, kl::dx::desc::subres* subData = nullptr);

		// Constant buffer
		kl::dx::buffer newCBuffer(uint byteSize);
		void setCBufferData(kl::dx::buffer buff, const void* data);
		void bindVertexCBuffer(kl::dx::buffer buff, uint slot);
		void bindPixelCBuffer(kl::dx::buffer buff, uint slot);
		template<typename T> inline bool autoVertexCBuffer(const T& data, uint slot = 0) {
			if ((!m_CBuffersPredefined) || (sizeof(T) > (KL_CBUFFER_PREDEFINED_SIZE * 16)) || (sizeof(T) % 16)) {
				return false;
			}
			kl::dx::buffer chosenBuffer = m_VertexCBuffers[(sizeof(T) / 16) - 1];
			bindVertexCBuffer(chosenBuffer, slot);
			setCBufferData(chosenBuffer, &data);
			return true;
		}
		template<typename T> inline bool autoPixelCBuffer(const T& data, uint slot = 0) {
			if ((!m_CBuffersPredefined) || (sizeof(T) > (KL_CBUFFER_PREDEFINED_SIZE * 16)) || (sizeof(T) % 16)) {
				return false;
			}
			kl::dx::buffer chosenBuffer = m_PixelCBuffers[(sizeof(T) / 16) - 1];
			bindPixelCBuffer(chosenBuffer, slot);
			setCBufferData(chosenBuffer, &data);
			return true;
		}

		// Vertex buffer
		kl::dx::mesh newVertexBuffer(const std::vector<kl::vertex>& vertexData);
		kl::dx::mesh newVertexBuffer(const std::string& filePath, bool flipZ = true);
		void draw(kl::dx::mesh mesh);

		// Sampler
		kl::dx::state::sampler newSamplerState(kl::dx::state::desc::sampler* desc);
		kl::dx::state::sampler newSamplerState(bool linear, bool mirror);
		void bind(kl::dx::state::sampler sampState, uint slot);

		// Texture
		kl::dx::texture newTextureBB();
		kl::dx::texture newTexture(kl::dx::desc::texture* desc, kl::dx::desc::subres* subData = nullptr);
		kl::dx::texture newTexture(const kl::image& img, bool enableUnorderedAccess = false);
		kl::dx::texture newTexture(const kl::image& front, const kl::image& back, const kl::image& left, const kl::image& right, const kl::image& top, const kl::image& bottom);
		kl::dx::texture newTextureST(kl::dx::texture tex, const kl::uint2& size = {});

		// Render target view
		kl::dx::view::target newTargetView(kl::dx::texture tex, kl::dx::view::desc::target* desc = nullptr);
		void clear(kl::dx::view::target view, const kl::float4& color);

		// Depth stencil view
		kl::dx::view::depth newDepthView(kl::dx::texture tex, kl::dx::view::desc::depth* desc = nullptr);
		void clear(kl::dx::view::depth view, float depth = 1.0f, byte stencil = 0);

		// Shader resource view
		kl::dx::view::shader newShaderView(kl::dx::texture tex, kl::dx::view::desc::shader* desc = nullptr);
		void bindPixelShaderView(kl::dx::view::shader view, uint slot);
		void bindComputeShaderView(kl::dx::view::shader view, uint slot);

		// Shader access view
		kl::dx::view::access newAccessView(kl::dx::texture tex, kl::dx::view::desc::access* desc = nullptr);
		void bindComputeAccessView(kl::dx::view::access view, uint slot, uint* initalCounts = nullptr);

		// Deletes child instance
		bool destroy(IUnknown* child);
	};
}
