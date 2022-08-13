#pragma once

#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>

#include "GPU/DXTypes.h"
#include "Render/Vertex.h"
#include "Graphics/Image.h"


namespace kl {
	struct Shaders {
		dx::VertexShader vertexShader = nullptr;
		dx::PixelShader pixelShader = nullptr;
		dx::Layout layout = nullptr;

		operator bool() const {
			return vertexShader && pixelShader && layout;
		}
	};
}

namespace kl {
	inline constexpr int CBUFFER_PREDEFINED_SIZE = 64;

	class GPU {
		dx::Device m_Device = nullptr;
		dx::Context m_Context = nullptr;
		dx::Chain m_Chain = nullptr;

		dx::TargetView m_FrameBuffer = nullptr;
		dx::DepthView m_DepthBuffer = nullptr;

		std::set<IUnknown*> m_Children;

		dx::Buffer m_VertexCBuffers[CBUFFER_PREDEFINED_SIZE] = {};
		dx::Buffer m_PixelCBuffers[CBUFFER_PREDEFINED_SIZE] = {};
		dx::Buffer m_ComputeCBuffers[CBUFFER_PREDEFINED_SIZE] = {};

	public:
		GPU(HWND window);
		GPU(const GPU&) = delete;
		void operator=(const GPU&) = delete;
		~GPU();

		dx::Device getDevice();
		const dx::Device getDevice() const;

		dx::Context getContext();
		const dx::Context getContext() const;

		void setViewport(const UInt2& size);
		void setViewport(const Int2& position, const UInt2& size);

		void unbindAllTargets();
		void bindInternalTargets();
		void bindTargets(const Vector<dx::TargetView>& targets, dx::DepthView depthView = nullptr);
		void bindTargetsWithInternal(const Vector<dx::TargetView>& additionalTargets, dx::DepthView depthView = nullptr);

		void resizeInternal(const UInt2& size);

		void clearInternalColor(const Float4& color);
		void clearInternalDepth(float value);
		void clearInternal();

		void swapBuffers(bool vSync);

		// Raster state
		dx::RasterState newRasterState(dx::RasterStateDesc* descriptor);
		dx::RasterState newRasterState(bool wireframe, bool cull, bool cullBack = true);
		void bindRasterState(dx::RasterState state);

		// Depth stenicl state
		dx::DepthState newDepthState(dx::DepthStateDesc* descriptor);
		dx::DepthState newDepthState(bool depth, bool stencil, bool mask);
		void bindDepthState(dx::DepthState state);

		// Shaders
		dx::VertexShader newVertexShader(const String& source, dx::Layout* outLayout = nullptr, const Vector<dx::LayoutDesc>& descriptors = {});
		dx::PixelShader newPixelShader(const String& source);
		dx::GeometryShader newGeometryShader(const String& source);
		dx::ComputeShader newComputeShader(const String& source);

		Shaders newShaders(const String& vertexSource, const String& pixelSource, const Vector<dx::LayoutDesc>& descriptors = {});
		Shaders newShaders(const String& source, const Vector<dx::LayoutDesc>& descriptors = {});

		void bindVertexShader(dx::VertexShader shader);
		void bindPixelShader(dx::PixelShader shader);
		void bindGeometryShader(dx::GeometryShader shader);
		void bindComputeShader(dx::ComputeShader shader);

		void bindLayout(dx::Layout layout);

		void bindShaders(const Shaders& shaders);

		void dispatchComputeShader(const UInt3& size);
		void executeComputeShader(dx::ComputeShader shader, const UInt3& size);

		void destroy(const Shaders& shaders);

		// Buffer
		dx::Buffer newBuffer(dx::BufferDesc* descriptor, dx::SubresDesc* subresourceData = nullptr);

		// Constant buffer
		dx::Buffer newCBuffer(uint byteSize);

		void setCBufferData(dx::Buffer cbuffer, const void* data);

		void bindVertexCBuffer(dx::Buffer cbuff, uint slot);
		void bindPixelCBuffer(dx::Buffer cbuff, uint slot);
		void bindComputeCBuffer(dx::Buffer cbuff, uint slot);

		template<typename T>
		inline bool autoVertexCBuffer(const T& data, uint slot = 0) {
			if (sizeof(T) > (CBUFFER_PREDEFINED_SIZE * 16) || sizeof(T) % 16) {
				return false;
			}
			dx::Buffer buffer = m_VertexCBuffers[sizeof(T) / 16 - 1];
			bindVertexCBuffer(buffer, slot);
			setCBufferData(buffer, &data);
			return true;
		}

		template<typename T>
		inline bool autoPixelCBuffer(const T& data, uint slot = 0) {
			if (sizeof(T) > (CBUFFER_PREDEFINED_SIZE * 16) || sizeof(T) % 16) {
				return false;
			}
			dx::Buffer buffer = m_PixelCBuffers[sizeof(T) / 16 - 1];
			bindPixelCBuffer(buffer, slot);
			setCBufferData(buffer, &data);
			return true;
		}

		template<typename T>
		inline bool autoComputeCBuffer(const T& data, uint slot = 0) {
			if (sizeof(T) > (CBUFFER_PREDEFINED_SIZE * 16) || sizeof(T) % 16) {
				return false;
			}
			dx::Buffer buffer = m_ComputeCBuffers[sizeof(T) / 16 - 1];
			bindComputeCBuffer(buffer, slot);
			setCBufferData(buffer, &data);
			return true;
		}

		// Vertex buffer
		dx::Buffer newVertexBuffer(const Vector<Vertex>& vertexData);
		dx::Buffer newVertexBuffer(const String& filePath, bool flipZ = true);
		
		dx::Buffer generateScreenMesh();
		dx::Buffer generatePlaneMesh(float size, int sizeInPoints);

		void drawVertexBuffer(dx::Buffer buffer);

		// Sampler
		dx::SamplerState newSamplerState(dx::SamplerStateDesc* desc);
		dx::SamplerState newSamplerState(bool linear, bool mirror);

		void bindSamplerState(dx::SamplerState state, uint slot);

		// Texture
		dx::Texture getBackBuffer();

		dx::Texture newTexture(dx::TextureDesc* descriptor, dx::SubresDesc* subresourceData = nullptr);

		dx::Texture newTexture(const Image& image, bool hasUnorderedAccess = false, bool isTarget = false);
		dx::Texture newTexture(const Image& front, const Image& back, const Image& left, const Image& right, const Image& top, const Image& bottom);

		dx::Texture newTextureStaging(dx::Texture texture, const UInt2& size = {});

		// Render target view
		dx::TargetView newTargetView(dx::Texture texture, dx::TargetViewDesc* descriptor = nullptr);

		void clearTargetView(dx::TargetView view, const Float4& color);

		// Depth stencil view
		dx::DepthView newDepthView(dx::Texture texture, dx::DepthViewDesc* desc = nullptr);

		void clearDepthView(dx::DepthView view, float depth = 1.0f, byte stencil = 0);

		// Shader resource view
		dx::ShaderView newShaderView(dx::Texture texture, dx::ShaderViewDesc* descriptor = nullptr);

		void bindPixelShaderView(dx::ShaderView view, uint slot);
		void bindComputeShaderView(dx::ShaderView view, uint slot);

		// Shader access view
		dx::AccessView newAccessView(dx::Texture texture, dx::AccessViewDesc* descriptor = nullptr);

		void bindComputeAccessView(dx::AccessView view, uint slot, uint* initalCounts = nullptr);

		// Deletes child instance
		void destroy(IUnknown* child);
	};
}
