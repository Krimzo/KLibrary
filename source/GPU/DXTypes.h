#pragma once

#include <d3d11.h>


namespace kl::dx {
	/* Types */
	// Device
	using Device = ID3D11Device*;
	using Context = ID3D11DeviceContext*;
	using Chain = IDXGISwapChain*;

	// Common
	using Buffer = ID3D11Buffer*;
	using Texture = ID3D11Texture2D*;

	// State
	using RasterState = ID3D11RasterizerState*;
	using DepthState = ID3D11DepthStencilState*;
	using SamplerState = ID3D11SamplerState*;
	using BlendState = ID3D11BlendState*;

	// View
	using TargetView = ID3D11RenderTargetView*;
	using DepthView = ID3D11DepthStencilView*;
	using ShaderView = ID3D11ShaderResourceView*;
	using AccessView = ID3D11UnorderedAccessView*;

	// Shader
	using VertexShader = ID3D11VertexShader*;
	using PixelShader = ID3D11PixelShader*;
	using GeometryShader = ID3D11GeometryShader*;
	using HullShader = ID3D11HullShader*;
	using DomainShader = ID3D11DomainShader*;
	using ComputeShader = ID3D11ComputeShader*;
	using Layout = ID3D11InputLayout*;

	/* Descriptors */
	// Common
	using BufferDesc = D3D11_BUFFER_DESC;
	using TextureDesc = D3D11_TEXTURE2D_DESC;
	using SubresDesc = D3D11_SUBRESOURCE_DATA;

	// State
	using RasterStateDesc = D3D11_RASTERIZER_DESC;
	using DepthStateDesc = D3D11_DEPTH_STENCIL_DESC;
	using SamplerStateDesc = D3D11_SAMPLER_DESC;
	using BlendStateDesc = D3D11_BLEND_DESC;

	// View
	using TargetViewDesc = D3D11_RENDER_TARGET_VIEW_DESC;
	using DepthViewDesc = D3D11_DEPTH_STENCIL_VIEW_DESC;
	using ShaderViewDesc = D3D11_SHADER_RESOURCE_VIEW_DESC;
	using AccessViewDesc = D3D11_UNORDERED_ACCESS_VIEW_DESC;

	// Shader
	using LayoutDesc = D3D11_INPUT_ELEMENT_DESC;
}
