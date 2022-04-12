#include "utility/file.h"


// Returns the file extension from the given file path
std::string kl::file::getExtension(const std::string& filePath)
{
	const size_t lastDotPos = filePath.find_last_of(".");
	if (lastDotPos == std::string::npos)
		return "";
	return filePath.substr(lastDotPos + 1);
}

// Returns the files in the given directory
std::vector<std::string> kl::file::getFiles(const std::string& dirPath, bool recursive)
{
	std::vector<std::string> files;
	if (recursive)
	{
		for (const auto& file : std::filesystem::recursive_directory_iterator(dirPath))
			if (!file.is_directory())
				files.push_back(file.path().string());
	}
	else
	{
		for (const auto& file : std::filesystem::directory_iterator(dirPath))
			if (!file.is_directory())
				files.push_back(file.path().string());
	}
	return files;
}

// Reads file data
std::string kl::file::read(const std::string& filePath)
{
	// Open file
	std::ifstream fileStream(filePath);
	std::stringstream textBuffer;
	if (!fileStream.is_open())
	{
		std::cout << "Could not open file \"" << filePath << "\"!" << std::endl;
		return {};
	}

	// Read/close
	textBuffer << fileStream.rdbuf();
	fileStream.close();
	return textBuffer.str();
}
std::vector<byte> kl::file::readB(const std::string& filePath)
{
	// Open file
	FILE* file = nullptr;
	fopen_s(&file, filePath.c_str(), "rb");
	if (!file)
	{
		std::cout << "Could not open file \"" << filePath << "\"!" << std::endl;
		return {};
	}

	// Seek to end and get pos
	fseek(file, 0, SEEK_END);
	const int byteSize = ftell(file);

	// Create buff and read data
	std::vector<byte> buff(byteSize);
	rewind(file);
	fread(&buff[0], 1, byteSize, file);

	// Close file
	fclose(file);

	// Return data
	return buff;
}

// Writes data to file
bool kl::file::write(const std::string& filePath, const std::string& data)
{
	// Open file
	std::ofstream fileStream(filePath);
	if (!fileStream.is_open())
	{
		std::cout << "Could not open file \"" << filePath << "\"!" << std::endl;
		return false;
	}

	// Write/close
	fileStream << data;
	fileStream.close();
	return true;
}
bool kl::file::writeB(const std::string& filePath, const std::vector<byte>& data)
{
	// Open file
	FILE* file = nullptr;
	fopen_s(&file, filePath.c_str(), "wb");
	if (!file)
	{
		std::cout << "Could not open file \"" << filePath << "\"!" << std::endl;
		return false;
	}

	// Write
	fwrite(&data[0], 1, data.size(), file);

	// Close
	fclose(file);
	return true;
}

// Appends text to a text file
bool kl::file::append(const std::string& filePath, const std::string& data, int position)
{
	// Open file
	std::fstream fileStream(filePath, std::ios::in | std::ios::out);
	if (!fileStream.is_open())
	{
		std::cout << "Could not open file \"" << filePath << "\"!" << std::endl;
		return false;
	}

	// Set pos
	if (position < 0)
		fileStream.seekp(0, std::ios_base::end);
	else
		fileStream.seekp(position);

	// Write and close
	fileStream << data;
	fileStream.close();
	return true;
}
bool kl::file::appendB(const std::string& filePath, const std::vector<byte>& data, int position)
{
	// Open file
	FILE* file = nullptr;
	fopen_s(&file, filePath.c_str(), "ab");
	if (!file)
	{
		std::cout << "Could not open file \"" << filePath << "\"!" << std::endl;
		return false;
	}

	// Set pos
	if (position < 0)
		fseek(file, 0, SEEK_END);
	else
		fseek(file, position, SEEK_SET);

	// Write
	fwrite(&data[0], 1, data.size(), file);

	// Close
	fclose(file);
	return true;
}

// Parses given .obj file
std::vector<kl::vertex> kl::file::parseObj(const std::string& filePath, bool flipZ)
{
	// Temp vertex buffer
	std::vector<kl::vertex> vertexData;

	// Opening the file
	std::fstream fileStream;
	fileStream.open(filePath, std::ios::in);
	if (!fileStream.is_open())
	{
		std::cout << "Could not open file \"" << filePath << "\"!" << std::endl;
		return {};
	}

	// Temp load buffers
	std::vector<kl::float3> xyzBuffer;
	std::vector<kl::float2> uvBuffer;
	std::vector<kl::float3> normBuffer;

	// Z flipper
	const int zFlip = flipZ ? -1 : 1;

	// Parsing data
	std::string fileLine;
	while (std::getline(fileStream, fileLine))
	{
		// Splitting the string by spaces
		std::vector<std::string> lineParts;
		std::stringstream lineStream(fileLine);
		for (std::string linePart; std::getline(lineStream, linePart, ' ');)
			lineParts.push_back(linePart);

		// Parsing the data
		if (lineParts[0] == "v")
			xyzBuffer.push_back(kl::float3(std::stof(lineParts[1]), std::stof(lineParts[2]), zFlip * std::stof(lineParts[3])));
		else if (lineParts[0] == "vt")
			uvBuffer.push_back(kl::float2(std::stof(lineParts[1]), std::stof(lineParts[2])));
		else if (lineParts[0] == "vn")
			normBuffer.push_back(kl::float3(std::stof(lineParts[1]), std::stof(lineParts[2]), zFlip * std::stof(lineParts[3])));
		else if (lineParts[0] == "f")
		{
			for (int i = 1; i < 4; i++)
			{
				// Getting the world, texture and normal indexes
				std::vector<std::string> linePartParts;
				std::stringstream linePartStream(lineParts[i]);
				for (std::string linePartPart; std::getline(linePartStream, linePartPart, '/');)
					linePartParts.push_back(linePartPart);

				// Saving the data
				vertexData.push_back(
					kl::vertex(
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

	// Data return
	return vertexData;
}
