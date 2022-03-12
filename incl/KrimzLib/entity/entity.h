#pragma once

#include "KrimzLib/gpu/gpu.h"
#include "KrimzLib/math/float3.h"
#include "KrimzLib/math/mat4.h"

#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>


namespace kl {
	class entity {
	public:
		// Name
		std::string name;

		// View
		bool visible = true;
		bool shadows = true;
		float roughness = 0.5f;

		// Geometry
		kl::float3 size = kl::float3(1);
		kl::float3 rotation;
		kl::float3 position;
		
		// Physics
		bool physics = true;
		kl::float3 acceler;
		kl::float3 velocity;
		kl::float3 angular;

		// Mesh pointer
		ID3D11Buffer* mesh = nullptr;

		// Texture pointer
		ID3D11ShaderResourceView* texture = nullptr;

		// Constructor
		entity(const std::string& name, ID3D11Buffer* mes, ID3D11ShaderResourceView* tex);

		// Updates the object physics
		void upPhys(float deltaT);

		// Returns the world matrix
		kl::mat4 matrix() const;

		// Renders the mesh
		void render(kl::gpu* gpu, bool useTex) const;
	};
}