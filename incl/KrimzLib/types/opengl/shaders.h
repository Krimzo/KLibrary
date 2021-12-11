#pragma once


namespace kl {
	struct shaders {
		// Constructor/destructor
		shaders(std::string vertexFile, std::string fragmentFile) {
			// File reading
			std::string vertexSource = kl::file::readText(vertexFile);
			std::string fragmentSource = kl::file::readText(fragmentFile);

			// Shader program creation
			shaderProgram = glCreateProgram();
			if (shaderProgram == NULL) {
				printf("Could not create a shader program!\n");
				exit(69);
			}

			// Compiling the vertex shader
			kl::id vertexShader = compileShader(vertexSource, GL_VERTEX_SHADER);

			// Compiling the fragment shader
			kl::id fragmentShader = compileShader(fragmentSource, GL_FRAGMENT_SHADER);

			// Attaching the shaders
			glAttachShader(shaderProgram, vertexShader);
			glAttachShader(shaderProgram, fragmentShader);

			// Linking the shaders
			glLinkProgram(shaderProgram);
			int linkStatus;
			glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkStatus);
			if (linkStatus == NULL) {
				char linkingInfo[1024];
				glGetProgramInfoLog(shaderProgram, sizeof(linkingInfo), NULL, linkingInfo);
				printf("Shader program linking error: '%s'\n", linkingInfo);
				exit(69);
			}

			// Validating the shader program
			glValidateProgram(shaderProgram);
			int validateStatus;
			glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &validateStatus);
			if (validateStatus == NULL) {
				char validateInfo[1024];
				glGetProgramInfoLog(shaderProgram, sizeof(validateInfo), NULL, validateInfo);
				printf("Shader program validation error: '%s'\n", validateInfo);
				exit(69);
			}

			// Cleanup
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);
		}
		~shaders() {
			glDeleteProgram(shaderProgram);
		}

		// Binds the shader program
		void bind() {
			glUseProgram(shaderProgram);
		}

		// Unbinds the shader program
		void unbind() {
			glUseProgram(NULL);
		}

	private:
		// Variables
		kl::id shaderProgram;

		// Returns a compiled shader
		kl::id compileShader(std::string shaderSource, kl::id shaderType) {
			// Creating the shader buffer
			kl::id shader = glCreateShader(shaderType);
			if (shader == NULL) {
				printf("Could not create a '%d' shader!\n", shaderType);
				exit(69);
			}

			// Setting and compiling the shader source
			const char* sourcePointer = shaderSource.c_str();
			const int sourceSize = (int)shaderSource.size();
			glShaderSource(shader, 1, &sourcePointer, &sourceSize);
			glCompileShader(shader);

			// Checking the compilation
			int compilationStatus;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &compilationStatus);
			if (compilationStatus == NULL) {
				char compilationInfo[1024];
				glGetShaderInfoLog(shader, sizeof(compilationInfo), NULL, compilationInfo);
				printf("'%d' shader compilation error: '%s'\n", shaderType, compilationInfo);
				exit(69);
			}

			// Returning the built shader
			return shader;
		}
	};
}
