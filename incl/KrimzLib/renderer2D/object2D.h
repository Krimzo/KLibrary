#pragma once

#include "KrimzLib/vecmath/vec2.h"
#include "KrimzLib/gl/mesh.h"
#include "KrimzLib/gl/texture.h"
#include "KrimzLib/vecmath/mat3.h"


namespace kl {
	class object2D {
	public:
		// View
		bool visible = true;

		// Geometry
		kl::vec2 size = kl::vec2(1);
		kl::vec2 position;
		float rotation = 0;
		
		// Physics
		bool physics = true;
		kl::vec2 acceler;
		kl::vec2 velocity;
		float angular = 0;

		// Mesh pointer
		kl::mesh* mesh = nullptr;

		// Texture pointer
		kl::texture* texture = nullptr;

		// Constructor
		object2D(kl::mesh* mes, kl::texture* tex);

		// Updates the object physics
		void upPhys(float deltaT);

		// Returns the world matrix
		kl::mat3 matrix() const;

		// Renders the mesh
		void render() const;
	};
}