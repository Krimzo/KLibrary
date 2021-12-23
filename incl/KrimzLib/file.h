#pragma once


namespace kl {
	namespace file {
		// Creates a file
		void create(std::string filePath) {
			std::ofstream(filePath).close();
		}

		// Opens a file with default program
		void openWithDefault(std::string filePath) {
			system(filePath.c_str());
		}

		// Opens a file with notepad
		void openWithNotepad(std::string filePath) {
			system(("notepad " + filePath).c_str());
		}

		// Opens a file with visual studio code
		void openWithVSCode(std::string filePath) {
			system(("code " + filePath).c_str());
		}

		// Returns the file extension from the given file path
		std::string getExtension(std::string filePath) {
			uint64_t lastDotPos = filePath.find_last_of(".");
			if (lastDotPos == std::string::npos) {
				return "";
			}
			return filePath.substr(lastDotPos + 1L);
		}

		// Returns a string from a given text file
		std::string readText(std::string filePath) {
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
		void writeText(std::string filePath, std::string& data) {
			std::ofstream fileStream(filePath);
			fileStream << data;
			fileStream.close();
		}
		void writeText(std::string filePath, std::string&& data) {
			writeText(filePath, data);
		}

		// Appends text to a text file
		void appendText(std::string filePath, std::string& data, int position = -1) {
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
		void appendText(std::string filePath, std::string&& data, int position = -1) {
			appendText(filePath, data, position);
		}

		// Returns a filedata from a given file
		kl::bytes readBytes(std::string filePath) {
			std::ifstream fileStream(filePath, std::ios::binary);
			if (!fileStream.is_open()) {
				printf("Could not load file \"%s\".\n", filePath.c_str());
				exit(69);
			}
			fileStream.seekg(0, std::ios::end);
			size_t fileLen = fileStream.tellg();
			fileStream.seekg(0, std::ios::beg);
			kl::bytes byteArray(fileLen);
			fileStream.read((char*)&byteArray[0], fileLen);
			fileStream.close();
			return byteArray;
		}

		// Writes bytes to a file
		void writeBytes(std::string filePath, kl::bytes& data) {
			std::ofstream fileStream(filePath, std::ios::binary);
			fileStream.write((char*)&data[0], data.size());
			fileStream.close();
		}
		void writeBytes(std::string filePath, kl::bytes&& data) {
			writeBytes(filePath, data);
		}

		// Appends bytes to a file
		void appendBytes(std::string filePath, kl::bytes& data, int position = -1) {
			std::fstream fileStream(filePath, std::ios::in | std::ios::out | std::ios::binary);
			if (!fileStream.is_open()) {
				printf("Could not load file \"%s\".\n", filePath.c_str());
				exit(69);
			}

			if (position < 0) {
				fileStream.seekp(0, std::ios_base::end);
			}
			else {
				fileStream.seekp(position);
			}

			fileStream.write((char*)&data[0], data.size());
			fileStream.close();
		}
		void appendBytes(std::string filePath, kl::bytes&& data, int position = -1) {
			appendBytes(filePath, data, position);
		}
	};
}
