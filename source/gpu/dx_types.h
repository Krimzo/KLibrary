#pragma once

#include <d3d11.h>


namespace kl::dx
{
	using device = ID3D11Device*;
	using context = ID3D11DeviceContext*;
	using chain = IDXGISwapChain*;

	using mesh = ID3D11Buffer*;
	using buffer = ID3D11Buffer*;
	using texture = ID3D11Texture2D*;

	namespace desc
	{
		using buffer = D3D11_BUFFER_DESC;
		using texture = D3D11_TEXTURE2D_DESC;
		using subres = D3D11_SUBRESOURCE_DATA;
	}

	namespace state
	{
		using raster = ID3D11RasterizerState*;
		using depth = ID3D11DepthStencilState*;
		using sampler = ID3D11SamplerState*;
		using blend = ID3D11BlendState*;

		namespace desc
		{
			using raster = D3D11_RASTERIZER_DESC;
			using depth = D3D11_DEPTH_STENCIL_DESC;
			using sampler = D3D11_SAMPLER_DESC;
			using blend = D3D11_BLEND_DESC;
		}
	}

	namespace view
	{
		using target = ID3D11RenderTargetView*;
		using depth = ID3D11DepthStencilView*;
		using shader = ID3D11ShaderResourceView*;
		using access = ID3D11UnorderedAccessView*;

		namespace desc
		{
			using target = D3D11_RENDER_TARGET_VIEW_DESC;
			using depth = D3D11_DEPTH_STENCIL_VIEW_DESC;
			using shader = D3D11_SHADER_RESOURCE_VIEW_DESC;
			using access = D3D11_UNORDERED_ACCESS_VIEW_DESC;
		}
	}

	namespace shader
	{
		using vertex = ID3D11VertexShader*;
		using pixel = ID3D11PixelShader*;
		using geometry = ID3D11GeometryShader*;
		using hull = ID3D11HullShader*;
		using domain = ID3D11DomainShader*;
		using compute = ID3D11ComputeShader*;

		using input = ID3D11InputLayout*;

		namespace desc
		{
			using input = D3D11_INPUT_ELEMENT_DESC;
		}
	}
}
