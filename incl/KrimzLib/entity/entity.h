#pragma once

#include "KrimzLib/dx/mesh.h"
#include "KrimzLib/dx/texture.h"
#include "KrimzLib/math/float3.h"
#include "KrimzLib/math/mat4.h"


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
		kl::mesh* mesh = nullptr;

		// Texture pointer
		kl::texture* texture = nullptr;

		// Constructor
		entity(const std::string& name, kl::mesh* mes, kl::texture* tex);

		// Updates the object physics
		void upPhys(float deltaT);

		// Returns the world matrix
		kl::mat4 matrix() const;

		// Renders the mesh
		void render(bool useTex) const;
	};
}