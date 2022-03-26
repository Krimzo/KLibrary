#include "KrimzLib/light/direct.h"


// Returns the true light color
kl::float4 kl::direct::getCol() const {
	return color * intensity;
}

// Returns the direction of light
kl::float3 kl::direct::getDir() const {
	return direction.normalize();
}

// Generates depth buffer
void kl::direct::genBuff(kl::gpu* gpu, int size) {
	// Texture gen
	D3D11_TEXTURE2D_DESC sunTexDesc = {};
	sunTexDesc.Width = 4096;
	sunTexDesc.Height = 4096;
	sunTexDesc.MipLevels = 1;
	sunTexDesc.ArraySize = 1;
	sunTexDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	sunTexDesc.SampleDesc.Count = 1;
	sunTexDesc.Usage = D3D11_USAGE_DEFAULT;
	sunTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	ID3D11Texture2D* sunTex = gpu->newTexture(&sunTexDesc);

	// Depth view gen
	D3D11_DEPTH_STENCIL_VIEW_DESC sunDepthVDesc = {};
	sunDepthVDesc.Format = DXGI_FORMAT_D32_FLOAT;
	sunDepthVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	this->shadowMapDV = gpu->newDepthView(sunTex, &sunDepthVDesc);

	// Shader view gen
	D3D11_SHADER_RESOURCE_VIEW_DESC sunShaderVDesc = {};
	sunShaderVDesc.Format = DXGI_FORMAT_R32_FLOAT;
	sunShaderVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	sunShaderVDesc.Texture2D.MipLevels = 1;
	this->shadowMapSV = gpu->newShaderView(sunTex, &sunShaderVDesc);

	// Cleanup
	gpu->destroy(sunTex);
}

// Returns the light vp matrix
kl::mat4 kl::direct::matrix(const kl::camera& cam) const {
	// Inverse camera matrix calculation
	const kl::mat4 invCam = (
		kl::mat4::persp(cam.fov, cam.aspect, cam.near, cam.shadows) *
		kl::mat4::lookAt(cam.position, cam.position + cam.getForward(), cam.getUp())
	).inverse();

	// Frustum world corners
	std::vector<kl::float4> frustumCorners;
	for (int x = 0; x < 2; x++) {
		for (int y = 0; y < 2; y++) {
			for (int z = 0; z < 2; z++) {
				const kl::float4 corner = invCam * kl::float4(2.0f * x - 1.0f, 2.0f * y - 1.0f, 2.0f * z - 1.0f, 1.0f);
				frustumCorners.push_back(corner / corner.w);
			}
		}
	}

	// Centroid calculation
	kl::float3 centroid;
	for (const auto& corn : frustumCorners) {
		centroid += corn.xyz();
	}
	centroid /= 8.0f;

	// Light view matrix
	const kl::mat4 view = kl::mat4::lookAt(centroid - this->getDir(), centroid, kl::float3::pos_y);

	// Finding min and max points
	kl::float3 minp(FLT_MAX, FLT_MAX, FLT_MAX);
	kl::float3 maxp(FLT_MIN, FLT_MIN, FLT_MIN);
	for (const auto& corn : frustumCorners) {
		const kl::float4 lightCorn = view * corn;
		minp.x = min(minp.x, lightCorn.x);
		maxp.x = max(maxp.x, lightCorn.x);
		minp.y = min(minp.y, lightCorn.y);
		maxp.y = max(maxp.y, lightCorn.y);
		minp.z = min(minp.z, lightCorn.z);
		maxp.z = max(maxp.z, lightCorn.z);
	}

	// Tunning near plane
	const float shadowMulti = 5.0f;
	maxp.z *= shadowMulti;

	// Light proj matrix
	const kl::mat4 proj = kl::mat4::ortho(minp.x, maxp.x, minp.y, maxp.y, maxp.z, minp.z);

	// Return
	return proj * view;
}
