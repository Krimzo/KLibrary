#include "KrimzLib/file.h"

#include <fstream>
#include <sstream>


// Returns the file extension from the given file path
std::string kl::file::getExtension(const std::string& filePath) {
	uint64_t lastDotPos = filePath.find_last_of(".");
	if (lastDotPos == std::string::npos) {
		return "";
	}
	return filePath.substr(lastDotPos + 1L);
}

// Returns a string from a given text file
std::string kl::file::read(const std::string& filePath) {
	std::ifstream fileStream(filePath);
	std::stringstream textBuffer;
	if (!fileStream.is_open()) {
		printf("Could not load text file \"%s\".\n", filePath.c_str());
		exit(69);
	}
	textBuffer << fileStream.rdbuf();
	fileStream.close();
	return textBuffer.str();
}

// Writes text to a text file
void kl::file::write(const std::string& filePath, const std::string& data) {
	std::ofstream fileStream(filePath);
	fileStream << data;
	fileStream.close();
}

// Appends text to a text file
void kl::file::append(const std::string& filePath, const std::string& data, int position = -1) {
	std::fstream fileStream(filePath, std::ios::in | std::ios::out);
	if (!fileStream.is_open()) {
		printf("Could not load text file \"%s\".\n", filePath.c_str());
		exit(69);
	}

	if (position < 0) {
		fileStream.seekp(0, std::ios_base::end);
	}
	else {
		fileStream.seekp(position);
	}

	fileStream << data;
	fileStream.close();
}
