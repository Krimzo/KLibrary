#pragma once

#include "KrimzLib/dx/mesh.h"
#include "KrimzLib/dx/texture.h"
#include "KrimzLib/vecmath/vec3.h"
#include "KrimzLib/vecmath/mat4.h"


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
		kl::vec3 size = kl::vec3(1);
		kl::vec3 rotation;
		kl::vec3 position;
		
		// Physics
		bool physics = true;
		kl::vec3 acceler;
		kl::vec3 velocity;
		kl::vec3 angular;

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