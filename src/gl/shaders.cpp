#include "KrimzLib/gl/shaders.h"


// Constructors/destructor
kl::shaders::shaders(const std::string& vertexSource, const std::string& fragmentSource) {
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
kl::shaders::shaders(const std::string& computeSource) {
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
kl::shaders::~shaders() {
	glDeleteProgram(programID);
}

// Binds the shader program
void kl::shaders::use() const {
	glUseProgram(programID);
}

// Returns the id of the given uniform name
kl::uniform kl::shaders::getUniform(const std::string& name) const {
	// Binding the shader program
	this->use();

	// Getting/checking the uniform location
	int uniformID = glGetUniformLocation(this->programID, name.c_str());
	if (uniformID == -1) {
		printf("Error: Uniform '%s' does not exist!\n", name.c_str());
		std::cin.get();
		exit(69);
	}

	// Returing a kl::uniform instance
	return kl::uniform(this->programID, uniformID);
}

// Parses a shader from a file
std::string kl::shaders::parse(const std::string& filePath, int type) {
	// Opening the file
	std::ifstream file(filePath);

	// Setting the types
	std::string parsingType;
	std::string nonParsingType1;
	std::string nonParsingType2;
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
	std::string line;
	while (std::getline(file, line)) {
		if (!storingData) {
			if (line.find(parsingType) != std::string::npos) {
				storingData = true;
			}
		}
		else {
			if (line.find(nonParsingType1) != std::string::npos || line.find(nonParsingType2) != std::string::npos) {
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

// Returns a compiled shader
kl::id kl::shaders::compileShader(const std::string& shaderSource, kl::id shaderType) {
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
		std::cin.get();
		exit(69);
	}

	// Returning the built shader
	return shader;
}

// Links the shaders and validates the shader program
void kl::shaders::finalizeProgram() {
	// Linking the shaders
	glLinkProgram(programID);
	int linkStatus;
	glGetProgramiv(programID, GL_LINK_STATUS, &linkStatus);
	if (!linkStatus) {
		char linkingInfo[1024];
		glGetProgramInfoLog(programID, sizeof(linkingInfo), NULL, linkingInfo);
		printf("Shader program linking error: '%s'\n", linkingInfo);
		std::cin.get();
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
		std::cin.get();
		exit(69);
	}
}
