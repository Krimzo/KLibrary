#pragma once


namespace kl {
	struct shaders {
		// Shader types
		enum type {
			Vertex = 1,
			Fragment = 2,
			Compute = 3
		};

		// Constructor/destructor
		shaders(String vertexSource, String fragmentSource) {
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
		shaders(String computeSource) {
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
		kl::uniform getUniform(String name) {
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

		// Parses a shader from a file
		static String parse(const String& filePath, int type) {
			// Opening the file
			std::ifstream file(filePath);

			// Setting the types
			String parsingType;
			String nonParsingType1;
			String nonParsingType2;
			if (type == kl::shaders::Vertex) {
				parsingType = "#shader vertex";
				nonParsingType1 = "#shader fragment";
				nonParsingType2 = "#shader compute";
			}
			else if (type == kl::shaders::Fragment) {
				parsingType = "#shader fragment";
				nonParsingType1 = "#shader vertex";
				nonParsingType2 = "#shader compute";
			}
			else if (type == kl::shaders::Compute) {
				parsingType = "#shader compute";
				nonParsingType1 = "#shader vertex";
				nonParsingType2 = "#shader fragment";
			}
			else {
				kl::console::error(true, "Shaders: Bad shader type given to a parser!");
			}

			// Shader data
			bool storingData = false;
			std::stringstream shaderData;

			// Looping through lines
			String line;
			while (std::getline(file, line)) {
				if (!storingData) {
					if (line.find(parsingType) != String::npos) {
						storingData = true;
					}
				}
				else {
					if (line.find(nonParsingType1) != String::npos || line.find(nonParsingType2) != String::npos) {
						break;
					}
					else {
						shaderData << line << '\n';
					}
				}
			}

			// Closing the file
			file.close();

			// Returning the shader data
			return shaderData.str();
		}

	private:
		// Variables
		kl::id programID = NULL;

		// Returns a compiled shader
		kl::id compileShader(String& shaderSource, kl::id shaderType) {
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
