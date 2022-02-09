#include "KrimzLib/gl/mesh.h"


// Constructors/destructor
kl::mesh::mesh(const std::vector<kl::vertex2D>& vertexData) {
	// Saving the vertex count
	this->vertexCount = (int)vertexData.size();

	// Generating the vao
	glGenVertexArrays(1, &this->vao);

	// Binding the vao
	glBindVertexArray(this->vao);

	// Generating the vbo
	glGenBuffers(1, &this->vbo);

	// Binding the vbo
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);

	// Sending the data to the vbo
	glBufferData(GL_ARRAY_BUFFER, this->vertexCount * sizeof(kl::vertex2D), &vertexData[0], GL_STATIC_DRAW);

	// Enabling the attributes
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	// Setting the data parsing type
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(kl::vertex2D), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(kl::vertex2D), (void*)sizeof(kl::vec2));

	// Unbinding the vao
	glBindVertexArray(NULL);

	// Disabling the attributes
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	// Unbinding the vbo
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
}
kl::mesh::mesh(const std::vector<kl::vertex3D>& vertexData) {
	// Saving the vertex count
	this->vertexCount = int(vertexData.size());

	// Generating the vao
	glGenVertexArrays(1, &this->vao);

	// Binding the vao
	glBindVertexArray(this->vao);

	// Generating the vbo
	glGenBuffers(1, &this->vbo);

	// Binding the vbo
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);

	// Sending the data to the vbo
	glBufferData(GL_ARRAY_BUFFER, this->vertexCount * sizeof(kl::vertex3D), &vertexData[0], GL_STATIC_DRAW);

	// Enabling the attributes
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// Setting the data parsing type
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(kl::vertex3D), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(kl::vertex3D), (void*)sizeof(kl::vec3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(kl::vertex3D), (void*)(sizeof(kl::vec3) + sizeof(kl::vec2)));

	// Unbinding the vao
	glBindVertexArray(NULL);

	// Disabling the attributes
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	// Unbinding the vbo
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
}
kl::mesh::mesh(const std::string& filePath, bool flipZ, bool load2D) {
	if (load2D) {
		loadFromFile2D(filePath);
	}
	else {
		loadFromFile3D(filePath, flipZ);
	}
}
kl::mesh::~mesh() {
	glDeleteVertexArrays(1, &this->vao);
	glDeleteBuffers(1, &this->vbo);
}

// Renders the mesh
void kl::mesh::draw() const {
	// Binding the vao
	glBindVertexArray(this->vao);

	// Drawing vertices
	glDrawArrays(GL_TRIANGLES, 0, this->vertexCount);

	// Unbinding the vao
	glBindVertexArray(NULL);
}

// Loads the vertex data from a .obj file
void kl::mesh::loadFromFile2D(const std::string& filePath) {
	// Temp vertex buffer
	std::vector<kl::vertex2D> vertexData;

	// Opening the file
	std::fstream fileStream;
	fileStream.open(filePath, std::ios::in);
	kl::console::error(!fileStream.is_open(), "Mesh: Could not open an object file!");

	// Temp load buffers
	std::vector<kl::vec2> xyBuffer;
	std::vector<kl::vec2> uvBuffer;

	// Parsing data
	std::string fileLine;
	while (std::getline(fileStream, fileLine)) {
		// Splitting the string by spaces
		std::vector<std::string> lineParts;
		std::stringstream lineStream(fileLine);
		for (std::string linePart; std::getline(lineStream, linePart, ' ');) {
			lineParts.push_back(linePart);
		}

		// Parsing the data
		if (lineParts[0] == "v") {
			xyBuffer.push_back(kl::vec2(std::stof(lineParts[1]), std::stof(lineParts[2])));
		}
		else if (lineParts[0] == "vt") {
			uvBuffer.push_back(kl::vec2(std::stof(lineParts[1]), std::stof(lineParts[2])));
		}
		else if (lineParts[0] == "f") {
			for (int i = 1; i < 4; i++) {
				// Getting the world and texture
				std::vector<std::string> linePartParts;
				std::stringstream linePartStream(lineParts[i]);
				for (std::string linePartPart; std::getline(linePartStream, linePartPart, '/');) {
					linePartParts.push_back(linePartPart);
				}

				// Saving the data
				vertexData.push_back(
					kl::vertex2D(
						xyBuffer[std::stoi(linePartParts[0]) - 1],
						uvBuffer[std::stoi(linePartParts[1]) - 1]
					)
				);
			}
		}
	}

	// Closing the file
	fileStream.close();

	// Loading the data
	this->mesh::mesh(vertexData);
}
void kl::mesh::loadFromFile3D(const std::string& filePath, bool flipZ) {
	// Temp vertex buffer
	std::vector<kl::vertex3D> vertexData;

	// Opening the file
	std::fstream fileStream;
	fileStream.open(filePath, std::ios::in);
	kl::console::error(!fileStream.is_open(), "Mesh: Could not open an object file!");

	// Temp load buffers
	std::vector<kl::vec3> xyzBuffer;
	std::vector<kl::vec2> uvBuffer;
	std::vector<kl::vec3> normBuffer;

	// Z flipper
	const int zFlip = flipZ ? -1 : 1;

	// Parsing data
	std::string fileLine;
	while (std::getline(fileStream, fileLine)) {
		// Splitting the string by spaces
		std::vector<std::string> lineParts;
		std::stringstream lineStream(fileLine);
		for (std::string linePart; std::getline(lineStream, linePart, ' ');) {
			lineParts.push_back(linePart);
		}

		// Parsing the data
		if (lineParts[0] == "v") {
			xyzBuffer.push_back(kl::vec3(std::stof(lineParts[1]), std::stof(lineParts[2]), zFlip * std::stof(lineParts[3])));
		}
		else if (lineParts[0] == "vt") {
			uvBuffer.push_back(kl::vec2(std::stof(lineParts[1]), std::stof(lineParts[2])));
		}
		else if (lineParts[0] == "vn") {
			normBuffer.push_back(kl::vec3(std::stof(lineParts[1]), std::stof(lineParts[2]), zFlip * std::stof(lineParts[3])));
		}
		else if (lineParts[0] == "f") {
			for (int i = 1; i < 4; i++) {
				// Getting the world, texture and normal indexes
				std::vector<std::string> linePartParts;
				std::stringstream linePartStream(lineParts[i]);
				for (std::string linePartPart; std::getline(linePartStream, linePartPart, '/');) {
					linePartParts.push_back(linePartPart);
				}

				// Saving the data
				vertexData.push_back(
					kl::vertex3D(
						xyzBuffer[std::stoi(linePartParts[0]) - 1],
						uvBuffer[std::stoi(linePartParts[1]) - 1],
						normBuffer[std::stoi(linePartParts[2]) - 1]
					)
				);
			}
		}
	}

	// Closing the file
	fileStream.close();

	// Loading the data
	this->mesh::mesh(vertexData);
}
