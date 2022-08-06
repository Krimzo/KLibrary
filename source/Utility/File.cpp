#include "utility/file.h"
#include "utility/console.h"
#include "utility/strings.h"


kl::String kl::Files::GetExtension(const String& filePath) {
	return std::filesystem::path(filePath).extension().string();
}

kl::Vector<kl::String> kl::Files::GetFiles(const String& dirPath, bool recursive) {
	Vector<String> files;
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

kl::String kl::Files::ReadString(const String& filePath) {
	std::ifstream fileStream(filePath);
	StringStream textBuffer;
	if (Warning(!fileStream.is_open(), "Failed to open file \"" + filePath + "\"")) {
		return {};
	}

	textBuffer << fileStream.rdbuf();
	fileStream.close();
	return textBuffer.str();
}

bool kl::Files::WriteString(const String& filePath, const String& data) {
	std::ofstream fileStream(filePath);
	if (Warning(!fileStream.is_open(), "Failed to open file \"" + filePath + "\"")) {
		return false;
	}

	fileStream << data;
	fileStream.close();
	return true;
}

bool kl::Files::AppendString(const String& filePath, const String& data, int position) {
	std::fstream fileStream(filePath, std::ios::in | std::ios::out);
	if (Warning(!fileStream.is_open(), "Failed to open file \"" + filePath + "\"")) {
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

kl::Vector<kl::Vertex> kl::Files::ParseMesh(const String& filePath, bool flipZ) {
	std::fstream fileStream;
	fileStream.open(filePath, std::ios::in);
	if (Warning(!fileStream.is_open(), "Failed to open file \"" + filePath + "\"")) {
		return {};
	}

	Vector<Vertex> vertexData;
	Vector<Float3> xyzBuffer;
	Vector<Float2> uvBuffer;
	Vector<Float3> normBuffer;

	const int zFlip = flipZ ? -1 : 1;
	for (String fileLine; std::getline(fileStream, fileLine);) {
		const Vector<String> lineParts = Strings::Split(fileLine, ' ');

		if (lineParts[0] == "v") {
			xyzBuffer.push_back(Float3(std::stof(lineParts[1]), std::stof(lineParts[2]), zFlip * std::stof(lineParts[3])));
		}
		else if (lineParts[0] == "vt") {
			uvBuffer.push_back(Float2(std::stof(lineParts[1]), std::stof(lineParts[2])));
		}
		else if (lineParts[0] == "vn") {
			normBuffer.push_back(Float3(std::stof(lineParts[1]), std::stof(lineParts[2]), zFlip * std::stof(lineParts[3])));
		}
		else if (lineParts[0] == "f") {
			for (int i = 1; i < 4; i++) {
				const Vector<String> linePartParts = Strings::Split(lineParts[i], '/');
				vertexData.push_back(
					kl::Vertex(
						xyzBuffer[std::stoull(linePartParts[0]) - 1],
						uvBuffer[std::stoull(linePartParts[1]) - 1],
						normBuffer[std::stoull(linePartParts[2]) - 1]
					)
				);
			}
		}
	}
	fileStream.close();
	return vertexData;
}

kl::File::File() {}

kl::File::File(const String& filePath, bool clear) {
	open(filePath, clear);
}

kl::File::~File() {
	close();
}

kl::File::operator bool() const {
	return isOpen();
}

bool kl::File::isOpen() const {
	return bool(m_File);
}

bool kl::File::open(const String& filePath, bool clear) {
	close();
	return !Warning(fopen_s(&m_File, filePath.c_str(), clear ? "wb+" : "ab+"), "Failed to open file \"" + filePath + "\"");
}

bool kl::File::close() {
	if (m_File) {
		fclose(m_File);
		m_File = nullptr;
		return true;
	}
	return false;
}

bool kl::File::seek(int64 position) {
	if (m_File) {
		if (position >= 0) {
			return !fseek(m_File, long(position), SEEK_SET);
		}
		return !fseek(m_File, long(position + 1), SEEK_END);
	}
	return false;
}

bool kl::File::move(int64 delta) {
	if (m_File) {
		return !fseek(m_File, long(delta), SEEK_CUR);
	}
	return false;
}

bool kl::File::rewind() {
	if (m_File) {
		seek(0);
		return true;
	}
	return false;
}

bool kl::File::unwind() {
	if (m_File) {
		seek(-1);
		return true;
	}
	return false;
}

kl::int64 kl::File::tell() const {
	if (m_File) {
		return ftell(m_File);
	}
	return -1;
}
