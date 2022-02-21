#pragma once 

#include "KrimzLib/vecmath/vec3.h"


namespace kl {
    class vertex {
	public:
		kl::vec3 world;
		kl::vec2 texture;
		kl::vec3 normal;

		// Constructors
		vertex();
		vertex(const kl::vec3& world);
		vertex(const kl::vec3& world, const kl::vec2& texture);
		vertex(const kl::vec3& world, const kl::vec3& normal);
		vertex(const kl::vec3& world, const kl::vec2& texture, const kl::vec3& normal);

		// Comparison
		bool equals(const kl::vertex& obj) const;
		bool operator==(const kl::vertex& obj) const;
		bool operator!=(const kl::vertex& obj) const;
	};
}