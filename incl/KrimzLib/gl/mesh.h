#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "KrimzLib/gl/gl.h"
#include "KrimzLib/renderer2D/vertex2D.h"
#include "KrimzLib/renderer3D/vertex3D.h"
#include "KrimzLib/console.h"


namespace kl {
	class mesh {
	public:
		// Constructors/destructor
		mesh(const std::vector<kl::vertex2D>& vertexData);
		mesh(const std::vector<kl::vertex3D>& vertexData);
		mesh(const std::string& filePath, bool flipZ, bool load2D);
		~mesh();

		// Renders the mesh
		void draw() const;

	private:
		kl::id vao = NULL;
		kl::id vbo = NULL;
		int vertexCount = 0;

		// Loads the vertex data from an .obj file
		void loadFromFile2D(const std::string& filePath);
		void loadFromFile3D(const std::string& filePath, bool flipZ);
	};
}
