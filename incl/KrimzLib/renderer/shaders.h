#pragma once


namespace kl {
	struct shaders {
		// Constructor/destructor
		shaders(std::string vertexSource, std::string fragmentSource) {
			// Shader program creation
			programID = glCreateProgram();
			kl::console::error(!programID, "OpenGL: Could not create a shader program!");

			// Compiling the shaders
			kl::id vertexShader = compileShader(vertexSource, GL_VERTEX_SHADER);
			kl::id fragmentShader = compileShader(fragmentSource, GL_FRAGMENT_SHADER);

			// Attaching the shaders
			glAttachShader(programID, vertexShader);
			glAttachShader(programID, fragmentShader);

			// Linking and validating the program
			finalizeProgram();

			// Cleanup
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);
		}
		shaders(std::string computeSource) {
			// Shader program creation
			programID = glCreateProgram();
			kl::console::error(!programID, "OpenGL: Could not create a shader program!");

			// Compiling the shaders
			kl::id computeShader = compileShader(computeSource, GL_COMPUTE_SHADER);

			// Attaching the shaders
			glAttachShader(programID, computeShader);

			// Linking and validating the program
			finalizeProgram();

			// Cleanup
			glDeleteShader(computeShader);
		}
		~shaders() {
			glDeleteProgram(programID);
		}

		// Binds the shader program
		void use() {
			glUseProgram(programID);
		}

		// Returns the id of the given uniform name
		kl::uniform getUniform(std::string name) {
			// Binding the shader program
			use();

			// Getting/checking the uniform location
			int uniformID = glGetUniformLocation(programID, name.c_str());
			if (uniformID == -1) {
				printf("Error: Uniform '%s' does not exist!\n", name.c_str());
				exit(69);
			}

			// Returing a kl::uniform instance
			return kl::uniform(programID, uniformID);
		}

	private:
		// Variables
		kl::id programID = NULL;

		// Returns a compiled shader
		kl::id compileShader(std::string& shaderSource, kl::id shaderType) {
			// Creating the shader buffer
			kl::id shader = glCreateShader(shaderType);
			kl::console::error(!shader, "OpenGL: Could not create a shader buffer!");

			// Setting and compiling the shader source
			const char* sourcePointer = shaderSource.c_str();
			const int sourceSize = (int)shaderSource.size();
			glShaderSource(shader, 1, &sourcePointer, &sourceSize);
			glCompileShader(shader);

			// Checking the compilation
			int compilationStatus;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &compilationStatus);
			if (!compilationStatus) {
				char compilationInfo[1024];
				glGetShaderInfoLog(shader, sizeof(compilationInfo), NULL, compilationInfo);
				printf("'%d' shader compilation error: '%s'\n", shaderType, compilationInfo);
				exit(69);
			}

			// Returning the built shader
			return shader;
		}

		// Links the shaders and validates the shader program
		void finalizeProgram() {
			// Linking the shaders
			glLinkProgram(programID);
			int linkStatus;
			glGetProgramiv(programID, GL_LINK_STATUS, &linkStatus);
			if (!linkStatus) {
				char linkingInfo[1024];
				glGetProgramInfoLog(programID, sizeof(linkingInfo), NULL, linkingInfo);
				printf("Shader program linking error: '%s'\n", linkingInfo);
				exit(69);
			}

			// Validating the shader program
			glValidateProgram(programID);
			int validateStatus;
			glGetProgramiv(programID, GL_VALIDATE_STATUS, &validateStatus);
			if (!validateStatus) {
				char validateInfo[1024];
				glGetProgramInfoLog(programID, sizeof(validateInfo), NULL, validateInfo);
				printf("Shader program validation error: '%s'\n", validateInfo);
				exit(69);
			}
		}
	};
}
