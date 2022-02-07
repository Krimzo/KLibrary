#pragma once 

#include "KrimzLib/vecmath/vec3.h"


namespace kl {
    class vertex3D {
	public:
		kl::vec3 world;
		kl::vec2 texture;
		kl::vec3 normal;

		// Constructors
		vertex3D();
		vertex3D(const kl::vec3& world);
		vertex3D(const kl::vec3& world, const kl::vec2& texture);
		vertex3D(const kl::vec3& world, const kl::vec2& texture, const kl::vec3& normal);

		// Comparison
		bool equals(const kl::vertex3D& obj) const;
		bool operator==(const kl::vertex3D& obj) const;
		bool operator!=(const kl::vertex3D& obj) const;

		// Prints the vertex
		void print() const;
	};
}