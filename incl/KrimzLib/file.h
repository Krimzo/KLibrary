#pragma once


namespace kl {
	namespace file {
		// Returns the file extension from the given file path
		String getExtension(const String& filePath) {
			uint64_t lastDotPos = filePath.find_last_of(".");
			if (lastDotPos == String::npos) {
				return "";
			}
			return filePath.substr(lastDotPos + 1L);
		}

		// Returns a string from a given text file
		String read(const String& filePath) {
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
		void write(const String& filePath, const String& data) {
			std::ofstream fileStream(filePath);
			fileStream << data;
			fileStream.close();
		}

		// Appends text to a text file
		void append(const String& filePath, const String& data, int position = -1) {
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
	};
}
