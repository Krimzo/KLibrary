#include "utility/file.h"
#include "utility/console.h"
#include "utility/strings.h"


std::string kl::file::extension(const std::string& filePath) {
	return std::filesystem::path(filePath).extension().string();
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

std::string kl::file::readString(const std::string& filePath) {
	std::ifstream fileStream(filePath);
	std::stringstream textBuffer;
	if (kl::console::warning(!fileStream.is_open(), "Failed to open file \"" + filePath + "\"")) {
		return {};
	}

	textBuffer << fileStream.rdbuf();
	fileStream.close();
	return textBuffer.str();
}

bool kl::file::writeString(const std::string& filePath, const std::string& data) {
	std::ofstream fileStream(filePath);
	if (kl::console::warning(!fileStream.is_open(), "Failed to open file \"" + filePath + "\"")) {
		return false;
	}

	fileStream << data;
	fileStream.close();
	return true;
}

bool kl::file::appendString(const std::string& filePath, const std::string& data, int position) {
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
		const std::vector<std::string> lineParts = kl::string::split(fileLine, ' ');

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
				const std::vector<std::string> linePartParts = kl::string::split(lineParts[i], '/');
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

kl::file::file() {}
kl::file::file(const std::string& filePath, bool clear) {
	open(filePath, clear);
}
kl::file::~file() {
	close();
}

kl::file::operator bool() const {
	return bool(m_File);
}

bool kl::file::open(const std::string& filePath, bool clear) {
	close();
	if (kl::console::warning(fopen_s(&m_File, filePath.c_str(), clear ? "wb+" : "ab+"), "Failed to open file \"" + filePath + "\"")) {
		return false;
	}
	return true;
}
bool kl::file::close() {
	if (m_File) {
		fclose(m_File);
		m_File = nullptr;
		return true;
	}
	return false;
}

bool kl::file::seek(int64 pos) {
	if (m_File) {
		if (pos >= 0) {
			return !fseek(m_File, long(pos), SEEK_SET);
		}
		return !fseek(m_File, long(pos + 1), SEEK_END);
	}
	return false;
}

bool kl::file::move(int64 delta) {
	if (m_File) {
		return !fseek(m_File, long(delta), SEEK_CUR);
	}
	return false;
}

bool kl::file::rewind() {
	if (m_File) {
		seek(0);
		return true;
	}
	return false;
}

bool kl::file::unwind() {
	if (m_File) {
		seek(-1);
		return true;
	}
	return false;
}

int64 kl::file::tell() const {
	if (m_File) {
		return ftell(m_File);
	}
	return -1;
}
