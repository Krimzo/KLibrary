#pragma once

#include <functional>
#include <vector>
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>

#include "KrimzLib/utility/convert.h"
#include "KrimzLib/math/float4.h"
#include "KrimzLib/math/mat4.h"
#include "KrimzLib/view/camera.h"
#include "KrimzLib/window/window.h"
#include "KrimzLib/gpu/gpu.h"


namespace kl {
	class direct {
	public:
		ID3D11DepthStencilView* shadowMapDV = nullptr;
		ID3D11ShaderResourceView* shadowMapSV = nullptr;
		kl::float4 color = kl::colors::white;
		kl::float3 direction = kl::float3(0.0f, -1.0f, -2.0f);
		float intensity = 1.0f;
		
		// Returns the true light color
		kl::float4 getCol() const;

		// Returns the direction of light
		kl::float3 getDir() const;

		// Generates depth buffer
		void genBuff(kl::gpu* gpu, int size);

		// Returns the light vp matrix
		kl::mat4 matrix(const kl::camera& cam) const;
	};
}
