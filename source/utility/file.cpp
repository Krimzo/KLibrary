#include "utility/file.h"

#include "utility/console.h"


std::string kl::file::extension(const std::string& filePath) {
	const uint64 lastDotPos = filePath.find_last_of(".");
	if (lastDotPos == std::string::npos) {
		return "";
	}
	return filePath.substr(lastDotPos + 1);
}

std::vector<std::string> kl::file::getFiles(const std::string& dirPath, bool recursive) {
	std::vector<std::string> files;
	if (recursive) {
		for (const auto& file : std::filesystem::recursive_directory_iterator(dirPath)) {
			if (!file.is_directory()) {
				files.push_back(file.path().string());
			}
		}
	}
	else {
		for (const auto& file : std::filesystem::directory_iterator(dirPath)) {
			if (!file.is_directory()) {
				files.push_back(file.path().string());
			}
		}
	}
	return files;
}

std::string kl::file::read(const std::string& filePath) {
	std::ifstream fileStream(filePath);
	std::stringstream textBuffer;
	if (kl::console::warning(!fileStream.is_open(), "Failed to open file \"" + filePath + "\"")) {
		return {};
	}

	textBuffer << fileStream.rdbuf();
	fileStream.close();
	return textBuffer.str();
}
std::vector<byte> kl::file::readB(const std::string& filePath) {
	FILE* file = nullptr;
	if (kl::console::warning(fopen_s(&file, filePath.c_str(), "rb"), "Failed to open file \"" + filePath + "\"")) {
		return {};
	}

	fseek(file, 0, SEEK_END);
	const int byteSize = ftell(file);

	std::vector<byte> buff(byteSize);
	rewind(file);
	fread(&buff[0], 1, byteSize, file);

	fclose(file);
	return buff;
}

bool kl::file::write(const std::string& filePath, const std::string& data) {
	std::ofstream fileStream(filePath);
	if (kl::console::warning(!fileStream.is_open(), "Failed to open file \"" + filePath + "\"")) {
		return false;
	}

	fileStream << data;
	fileStream.close();
	return true;
}
uint64 kl::file::writeB(const std::string& filePath, const std::vector<byte>& data) {
	FILE* file = nullptr;
	if (kl::console::warning(fopen_s(&file, filePath.c_str(), "wb"), "Failed to open file \"" + filePath + "\"")) {
		return 0;
	}

	const uint64 bytesWritten = fwrite(&data[0], 1, data.size(), file);

	fclose(file);
	return bytesWritten;
}

bool kl::file::append(const std::string& filePath, const std::string& data, int position) {
	std::fstream fileStream(filePath, std::ios::in | std::ios::out);
	if (kl::console::warning(!fileStream.is_open(), "Failed to open file \"" + filePath + "\"")) {
		return false;
	}

	if (position < 0) {
		fileStream.seekp(0, std::ios_base::end);
	}
	else {
		fileStream.seekp(position);
	}
	fileStream << data;

	fileStream.close();
	return true;
}
uint64 kl::file::appendB(const std::string& filePath, const std::vector<byte>& data, int position) {
	FILE* file = nullptr;
	if (kl::console::warning(fopen_s(&file, filePath.c_str(), "ab"), "Failed to open file \"" + filePath + "\"")) {
		return 0;
	}

	if (position < 0) {
		fseek(file, 0, SEEK_END);
	}
	else {
		fseek(file, position, SEEK_SET);
	}
	const uint64 bytesWritten = fwrite(&data[0], 1, data.size(), file);

	fclose(file);
	return bytesWritten;
}

std::vector<kl::vertex> kl::file::parseMesh(const std::string& filePath, bool flipZ) {
	std::fstream fileStream;
	fileStream.open(filePath, std::ios::in);
	if (kl::console::warning(!fileStream.is_open(), "Failed to open file \"" + filePath + "\"")) {
		return {};
	}

	std::vector<kl::vertex> vertexData;
	std::vector<kl::float3> xyzBuffer;
	std::vector<kl::float2> uvBuffer;
	std::vector<kl::float3> normBuffer;

	const int zFlip = flipZ ? -1 : 1;
	for (std::string fileLine; std::getline(fileStream, fileLine);) {
		std::vector<std::string> lineParts;
		std::stringstream lineStream(fileLine);
		for (std::string linePart; std::getline(lineStream, linePart, ' ');) {
			lineParts.push_back(linePart);
		}

		if (lineParts[0] == "v") {
			xyzBuffer.push_back(kl::float3(std::stof(lineParts[1]), std::stof(lineParts[2]), zFlip * std::stof(lineParts[3])));
		}
		else if (lineParts[0] == "vt") {
			uvBuffer.push_back(kl::float2(std::stof(lineParts[1]), std::stof(lineParts[2])));
		}
		else if (lineParts[0] == "vn") {
			normBuffer.push_back(kl::float3(std::stof(lineParts[1]), std::stof(lineParts[2]), zFlip * std::stof(lineParts[3])));
		}
		else if (lineParts[0] == "f") {
			for (int i = 1; i < 4; i++) {
				std::vector<std::string> linePartParts;
				std::stringstream linePartStream(lineParts[i]);
				for (std::string linePartPart; std::getline(linePartStream, linePartPart, '/');) {
					linePartParts.push_back(linePartPart);
				}

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

	fileStream.close();
	return vertexData;
}
