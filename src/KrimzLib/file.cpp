#include "KrimzLib/file.h"


// Returns the file extension from the given file path
std::string kl::file::getExtension(const std::string& filePath) {
	const size_t lastDotPos = filePath.find_last_of(".");
	if (lastDotPos == std::string::npos) {
		return "";
	}
	return filePath.substr(lastDotPos + 1);
}

// Returns the files in the given directory
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

// Returns a string from a given text file
std::string kl::file::read(const std::string& filePath) {
	std::ifstream fileStream(filePath);
	std::stringstream textBuffer;
	if (!fileStream.is_open()) {
		printf("Could not load text file \"%s\".\n", filePath.c_str());
		std::cin.get();
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
void kl::file::append(const std::string& filePath, const std::string& data, int position) {
	std::fstream fileStream(filePath, std::ios::in | std::ios::out);
	if (!fileStream.is_open()) {
		printf("Could not load text file \"%s\".\n", filePath.c_str());
		std::cin.get();
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
