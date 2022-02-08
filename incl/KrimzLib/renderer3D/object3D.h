#pragma once

#include "KrimzLib/vecmath/vec3.h"
#include "KrimzLib/vecmath/mat4.h"
#include "KrimzLib/gl/mesh.h"
#include "KrimzLib/gl/texture.h"


namespace kl {
	class object3D {
	public:
		// View
		bool visible = true;
		bool shadows = true;

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
		object3D(kl::mesh* mes, kl::texture* tex);

		// Updates the object physics
		void upPhys(float deltaT);

		// Returns the world matrix
		kl::mat4 matrix() const;

		// Renders the mesh
		void render() const;
	};
}