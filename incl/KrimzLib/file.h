#pragma once


namespace kl {
	namespace file {
		// Creates a file
		void create(std::string filePath) {
			std::ofstream(filePath).close();
		}

		// Opens a file with default program
		void openDefault(std::string filePath) {
			system(filePath.c_str());
		}

		// Opens a file with notepad
		void openNotepad(std::string filePath) {
			system(("notepad " + filePath).c_str());
		}

		// Opens a file with visual studio code
		void openVSCode(std::string filePath) {
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
		std::string read(std::string filePath) {
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
		void write(std::string filePath, std::string& data) {
			std::ofstream fileStream(filePath);
			fileStream << data;
			fileStream.close();
		}
		void write(std::string filePath, std::string&& data) {
			write(filePath, data);
		}

		// Appends text to a text file
		void append(std::string filePath, std::string& data, int position = -1) {
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
		void append(std::string filePath, std::string&& data, int position = -1) {
			append(filePath, data, position);
		}
	};
}
