#pragma once


namespace kl {
	struct shaders {
		// Constructor/destructor
		shaders(std::string vertexSource, std::string fragmentSource) {
			// Shader program creation
			shaderProgram = glCreateProgram();
			if (shaderProgram == NULL) {
				printf("Shader program creation error!\n");
				exit(69);
			}

			// Compiling the shaders
			kl::id vertexShader = compileShader(vertexSource, GL_VERTEX_SHADER);
			kl::id fragmentShader = compileShader(fragmentSource, GL_FRAGMENT_SHADER);

			// Attaching the shaders
			glAttachShader(shaderProgram, vertexShader);
			glAttachShader(shaderProgram, fragmentShader);

			// Linking and validating the program
			finalizeProgram();

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

		// Returns the id of the given uniform name
		int getUniform(std::string name) {
			int uniformID = glGetUniformLocation(shaderProgram, name.c_str());
			if (uniformID == -1) {
				printf("Error: Uniform '%s' doesn't exist!\n", name.c_str());
				exit(69);
			}
			return uniformID;
		}

		// Setts the data of an uniform
		void setUniform(int id, int data) {
			glUniform1i(id, data);
		}
		void setUniform(int id, float data) {
			glUniform1f(id, data);
		}
		void setUniform(int id, kl::vec2 data) {
			glUniform2f(id, data.x, data.y);
		}
		void setUniform(int id, kl::vec3 data) {
			glUniform3f(id, data.x, data.y, data.z);
		}
		void setUniform(int id, kl::vec4 data) {
			glUniform4f(id, data.x, data.y, data.z, data.w);
		}
		void setUniform(int id, kl::mat4& data) {
			glUniformMatrix4fv(id, 1, true, data.pointer());
		}
		void setUniform(int id, kl::mat4&& data) {
			setUniform(id, data);
		}

	private:
		// Variables
		kl::id shaderProgram;

		// Returns a compiled shader
		kl::id compileShader(std::string& shaderSource, kl::id shaderType) {
			// Creating the shader buffer
			kl::id shader = glCreateShader(shaderType);
			if (shader == NULL) {
				printf("'%d' shader creation error\n", shaderType);
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

		// Links the shaders and validates the shader program
		void finalizeProgram() {
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
		}
	};
}
