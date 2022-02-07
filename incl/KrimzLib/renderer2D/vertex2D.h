#pragma once 

#include <iostream>

#include "KrimzLib/vecmath/vec2.h"


namespace kl {
	class vertex2D {
	public:
		kl::vec2 world;
		kl::vec2 texture;

		// Constructors
		vertex2D();
		vertex2D(const kl::vec2& world);
		vertex2D(const kl::vec2& world, const kl::vec2& texture);

		// Comparison
		bool equals(const kl::vertex2D& obj) const;
		bool operator==(const kl::vertex2D& obj) const;
		bool operator!=(const kl::vertex2D& obj) const;

		// Prints the vertex
		void print() const;
	};
}