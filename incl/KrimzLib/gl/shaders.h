#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "KrimzLib/gl/gl.h"
#include "KrimzLib/console.h"
#include "KrimzLib/gl/uniform.h"


namespace kl {
	class shaders {
	public:
		// Shader types
		enum type {
			Vertex = 1,
			Fragment = 2,
			Compute = 3
		};

		// Constructors/destructor
		shaders(const std::string& vertexSource, const std::string& fragmentSource);
		shaders(const std::string& computeSource);
		~shaders();

		// Binds the shader program
		void use() const;

		// Returns the id of the given uniform name
		kl::uniform getUniform(const std::string& name) const;

		// Parses a shader from a file
		static std::string parse(const std::string& filePath, int type);

	private:
		// Variables
		kl::id programID = NULL;

		// Returns a compiled shader
		kl::id compileShader(const std::string& shaderSource, kl::id shaderType);

		// Links the shaders and validates the shader program
		void finalizeProgram();
	};
}
